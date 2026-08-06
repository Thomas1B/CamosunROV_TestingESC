# CamosunROV Testing ESC

Firmware for driving Afro30A / BlueRobotics Basic ESCs from an STM32F446xx, using hardware PWM to send standard 1100–1900 µs throttle pulses.

<hr>
Product:

- [BlueRobotics Basic ESC (BESC30 R3)](https://bluerobotics.com/store/thrusters/speed-controllers/besc30-r3/)
- [Afro 30A ESC datasheet](https://arduino.ua/docs/AfroESC30A.pdf)
<hr>

## ESC control

Each ESC is driven by a PWM channel updated through one of six functions in `motors.c` — `motor1()` through `motor6()`, one per motor — each taking a single `throttlePercent` argument (`int32_t`). A throttle value of `-100` to `100` is linearly mapped to a `1100–1900 µs` pulse width (1500 µs = neutral/stop), then written directly into the timer's capture/compare register. Values outside `-100`–`100` are clamped before mapping, so an out-of-range input saturates at the min/max pulse width instead of being extrapolated into an invalid compare value:

```c
static inline uint32_t throttleToPulse(int32_t throttlePercent) {
    if (throttlePercent > 100) {
        throttlePercent = 100;
    } else if (throttlePercent < -100) {
        throttlePercent = -100;
    }
    return lroundf(map(throttlePercent, -100.0f, 100.0f, 1100.0f, 1900.0f));
}

static inline void motor_write(TIM_HandleTypeDef *htim, uint32_t channel, int32_t throttlePercent) {
    __HAL_TIM_SET_COMPARE(htim, channel, throttleToPulse(throttlePercent));
}
```

`reset_motors()` zeroes all six motors, re-enables each timer's main output (MOE), and clears any latched break flag — call this after an emergency stop or a break-input fault.

### Motor → timer/channel mapping

| Motor | Timer | Channel |
|---|---|---|
| 1 | TIM1 | 1 |
| 2 | TIM1 | 2 |
| 3 | TIM1 | 3 |
| 4 | TIM1 | 4 |
| 5 | TIM8 | 1 |
| 6 | TIM8 | 2 |

TIM1 and TIM8 are both advanced-control timers with break-input support, which this project uses as a hardware fault line (`TIM_IT_BREAK`). The break input (BKIN) on each timer is wired to the hull's leak sensor and is configured active-high (`BreakPolarity = TIM_BREAKPOLARITY_HIGH`), so a leak sensor output going high directly and immediately cuts all PWM output at the timer hardware level, regardless of what the firmware is doing at that instant. `HAL_TIMEx_BreakCallback()` then fires to light an indicator LED. `reset_motors()` is required to clear the latched break flag and re-arm the outputs once the fault condition has cleared.

## PWM timer settings

TIM1 and TIM8 are configured identically in `main.c`:

| Setting | Value |
|---|---|
| Prescaler | 180 |
| Period (ARR) | 3333 |
| Counter mode | Up |
| Clock division | 1 |
| OC mode | PWM1 |
| OC polarity | High |

Both timers sit on APB2. With the system clock configuration in `SystemClock_Config()` (HSI → PLL, `PLLM=8`, `PLLN=180`, `PLLP=2`, giving a 180 MHz SYSCLK, and `APB2CLKDivider = DIV2`), the APB2 timer clock feeding TIM1/TIM8 is **180 MHz**.

That yields:

- **Timer tick:** (180 + 1) / 180 MHz ≈ **1.0056 µs**
- **PWM period:** (3333 + 1) ticks × 1.0056 µs ≈ **3.35 ms**
- **PWM frequency:** ≈ **298 Hz**

The 1100–1900 µs throttle range maps to compare-register values of roughly 1094–1889 counts, well within the 3333-count period, leaving headroom above the ESC's expected pulse range.

Break/dead-time config (`sBreakDeadTimeConfig`) uses `TIM_BREAK_ENABLE` with `DeadTime = 0` (no dead-time insertion needed, since these are single, non-complementary PWM outputs to each ESC) and `AutomaticOutput` disabled, so the main output must be re-enabled in software (`__HAL_TIM_MOE_ENABLE`) after a break — handled by `reset_motors()`.
