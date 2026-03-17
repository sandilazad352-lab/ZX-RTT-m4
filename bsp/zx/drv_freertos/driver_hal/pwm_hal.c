/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <string.h>

#include "aic_core.h"
#include "aic_hal_clk.h"

#include "pwm_hal.h"

/* Register definition of PWM Controller */

#define PWM_PWMx 0x300

#define PWM_CTL 0x000
#define PWM_MCTL 0x004
#define PWM_CKCTL 0x008
#define PWM_INTCTL 0x00C
#define PWM_INTSTS 0x010
#define PWM_TBCTL(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x000)
#define PWM_TBSTS(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x004)
#define PWM_TBPHS(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x008)
#define PWM_TBCTR(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x010)
#define PWM_TBPRD(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x014)
#define PWM_CMPCTL(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x018)
#define PWM_CMPAHR(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x01C)
#define PWM_CMPA(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x020)
#define PWM_CMPB(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x024)
#define PWM_AQCTLA(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x028)
#define PWM_AQCTLB(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x02C)
#define PWM_AQSFRC(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x030)
#define PWM_AQCSFRC(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x034)
#define PWM_DBCTL(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x038)
#define PWM_DBRED(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x03C)
#define PWM_DBFED(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x040)
#define PWM_ETSEL(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x044)
#define PWM_ETPS(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x048)
#define PWM_ETFLG(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x04C)
#define PWM_ETCLR(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x050)
#define PWM_ETFRC(n) (PWM_PWMx + (((n) & 0x7) << 8) + 0x054)
#define PWM_VERSION 0xFFC

#define PWM_DEFAULT_TB_CLK_RATE 24000000
#define PWM_DEFAULT_DB_RED 20
#define PWM_DEFAULT_DB_FED 20

#define PWM_ACTION_CFG_NUM 6

#define PWM_CTL_EN BIT(0)
#define PWM_MCTL_PWM0_EN BIT(0)
#define PWM_MCTL_PWM_EN(n) (PWM_MCTL_PWM0_EN << (n))
#define PWM_CKCTL_PWM0_ON BIT(0)
#define PWM_CKCTL_PWM_ON(n) (PWM_CKCTL_PWM0_ON << (n))
#define PWM_INTCTL_PWM0_ON BIT(0)
#define PWM_INTCTL_PWM_ON(n) (PWM_INTCTL_PWM0_ON << (n))
#define PWM_TBCTL_CLKDIV_MAX 0xFFF
#define PWM_TBCTL_CLKDIV_SHIFT 16
#define PWM_TBCTL_CTR_MODE_MASK GENMASK(1, 0)
#define PWM_TBPRD_MAX 0xFFFF
#define PWM_AQCTL_DEF_LEVEL BIT(16)
#define PWM_AQCTL_CBD_SHIFT 10
#define PWM_AQCTL_CBU_SHIFT 8
#define PWM_AQCTL_CAD_SHIFT 6
#define PWM_AQCTL_CAU_SHIFT 4
#define PWM_AQCTL_PRD_SHIFT 2
#define PWM_AQCTL_MASK 0x3
#define PWM_ETSEL_INTEN_SHIFT 3
#define PWM_ETSEL_INTSEL_SHIFT 0
#define PWM_SHADOW_SEL_ZRQ_PRD 0xa

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000UL
#endif

#ifndef AIC_PWM_CH_NUM
#define AIC_PWM_CH_NUM 4
#endif

static struct aic_pwm_arg g_pwm_args[AIC_PWM_CH_NUM] = {{0}};

static inline void pwm_writel(uint32_t val, int reg) {
    writel(val, PWM_BASE + reg);
}

static inline uint32_t pwm_readl(int reg) {
    return readl(PWM_BASE + reg);
}

static void pwm_reg_enable(int offset, int bit, int enable) {
    int tmp;

    tmp = pwm_readl(offset);
    tmp &= ~bit;
    if (enable)
        tmp |= bit;

    pwm_writel(tmp, offset);
}

uint32_t pwm_hal_int_sts(void) {
    return pwm_readl(PWM_INTSTS);
}

void pwm_hal_clr_int(uint32_t stat) {
    pwm_writel(stat, PWM_INTSTS);
}

void pwm_hal_int_config(uint32_t ch, u8 irq_mode, u8 enable) {
    pwm_writel((enable << PWM_ETSEL_INTEN_SHIFT) | ((irq_mode + 4) << PWM_ETSEL_INTSEL_SHIFT), PWM_ETSEL(ch));
    pwm_reg_enable(PWM_INTCTL, PWM_INTCTL_PWM_ON(ch), enable);
}

int pwm_hal_ch_init(uint32_t ch, enum aic_pwm_mode mode) {
    struct aic_pwm_arg* arg = &g_pwm_args[ch];
    arg->mode = mode;
    arg->available = 1;
    pwm_reg_enable(PWM_CKCTL, PWM_CKCTL_PWM_ON(ch), 1);
    pwm_writel(PWM_SHADOW_SEL_ZRQ_PRD, PWM_CMPCTL(ch));

    uint32_t div = 0;
    div = PWM_CLK_RATE / PWM_TB_CLK_RATE - 1;
    if (div > PWM_TBCTL_CLKDIV_MAX) {
        hal_log_err("ch%ld clkdiv %ld is too big\n", ch, div);
        return -ERANGE;
    }
    pwm_writel((div << PWM_TBCTL_CLKDIV_SHIFT) | arg->mode, PWM_TBCTL(ch));
    return 0;
}

int pwm_hal_get(uint32_t ch, uint32_t* duty_ns, uint32_t* period_ns) {
    struct aic_pwm_arg* arg = &g_pwm_args[ch];

    if (!arg->available) {
        hal_log_err("ch%ld is unavailable\n", ch);
        return -EINVAL;
    }

    *duty_ns = arg->duty;
    *period_ns = arg->period;
    return 0;
}

int pwm_hal_signal_enable(uint32_t ch, uint8_t signal, uint8_t enable) {
    uint32_t offset;
    uint16_t shift = 0;

    if (signal == 0) {
        shift = PWM_AQCTL_CAU_SHIFT;
        offset = PWM_AQCTLA(ch);
    } else {
        shift = PWM_AQCTL_CBU_SHIFT;
        offset = PWM_AQCTLB(ch);
    }
    uint32_t action = pwm_readl(offset);
    action &= ~0xfff;
    if (enable == 1) {
        uint8_t defult_level = action & PWM_AQCTL_DEF_LEVEL ? 1 : 0;
        if (defult_level) {
            action |= (PWM_ACT_LOW << shift) | PWM_ACT_HIGH;
        } else {
            action |= (PWM_ACT_HIGH << shift) | PWM_ACT_LOW;
        }
    }
    pwm_writel(action, offset);
    return 0;
}

int pwm_hal_enable(uint32_t ch, uint8_t enable) {
    struct aic_pwm_arg* arg = &g_pwm_args[ch];

    if (!arg->available) {
        hal_log_err("ch%ld is unavailable\n", ch);
        return -EINVAL;
    }

    hal_log_debug("ch%ld enable\n", ch);
    pwm_reg_enable(PWM_MCTL, PWM_MCTL_PWM_EN(ch), enable ? 1 : 0);
    return 0;
}

int pwm_hal_set_period_ns(uint32_t ch, uint32_t ns) {
    struct aic_pwm_arg* arg = &g_pwm_args[ch];
    uint32_t prd = 0;
    if ((ns < 1) || (ns > NSEC_PER_SEC)) {
        hal_log_err("ch%ld invalid period %ld\n", ch, ns);
        return -ERANGE;
    }
    arg->freq = NSEC_PER_SEC / ns;
    prd = PWM_TB_CLK_RATE / arg->freq;
    if (arg->mode == PWM_MODE_UP_DOWN_COUNT) {
        prd >>= 1;
    } else {
        prd--;
    }
    if (prd > PWM_TBPRD_MAX) {
        hal_log_err("ch%ld period %ld is too big\n", ch, prd);
        return -ERANGE;
    }
    arg->period = prd;
    arg->period_ns = NSEC_PER_SEC / (PWM_TB_CLK_RATE / (prd + 1));
    pwm_writel(prd, PWM_TBPRD(ch));
    return 0;
}

int pwm_hal_signal_set_duty_ns(uint32_t ch, uint8_t signal, uint32_t duty_ns) {
    struct aic_pwm_arg* arg = &g_pwm_args[ch];
    uint32_t prd = arg->period;
    u64 duty = 0;
    duty = (uint64_t)duty_ns * prd / arg->period_ns;
    if (duty == prd) {
        duty++;
    }
    hal_log_debug("Set CMP %ld/%ld\n", (uint32_t)duty, prd);
    if (signal == 0) {
        pwm_writel((uint32_t)duty, PWM_CMPA(ch));
    } else {
        pwm_writel((uint32_t)duty, PWM_CMPB(ch));
    }
    return 0;
}

int pwm_hal_signal_set_default_level(uint32_t ch, uint32_t signal, uint32_t level) {
    uint32_t offset;
    if (signal == 0) {
        offset = PWM_AQCTLA(ch);
    } else {
        offset = PWM_AQCTLB(ch);
    }
    uint32_t action = pwm_readl(offset);
    uint8_t defult_level = action & PWM_AQCTL_DEF_LEVEL ? 1 : 0;
    if (defult_level == level) {
        return 0;
    }

    if (level) {
        action |= PWM_AQCTL_DEF_LEVEL;
    } else {
        action &= ~PWM_AQCTL_DEF_LEVEL;
    }
    pwm_writel(action, offset);

    // force update action level
    if (action & 0x02) {
        pwm_hal_signal_enable(ch, signal, 1);
    }

    return 0;
}

int pwm_hal_init(void) {
    int i, ret = 0;

    ret = hal_clk_set_freq(CLK_PWM, PWM_CLK_RATE);
    if (ret < 0) {
        hal_log_err("Failed to set PWM clk %d\n", PWM_CLK_RATE);
        return -1;
    }
    ret = hal_clk_enable(CLK_PWM);
    if (ret < 0) {
        hal_log_err("Failed to enable PWM clk\n");
        return -1;
    }

    ret = hal_clk_enable_deassertrst(CLK_PWM);
    if (ret < 0) {
        hal_log_err("Failed to reset PWM deassert\n");
        return -1;
    }

    pwm_reg_enable(PWM_CTL, PWM_CTL_EN, 1);

    /* default configuration */
    for (i = 0; i < AIC_PWM_CH_NUM; i++) {
        g_pwm_args[i].id = i;
        g_pwm_args[i].mode = PWM_MODE_UP_COUNT;
        g_pwm_args[i].tb_clk_rate = PWM_DEFAULT_TB_CLK_RATE;
    }

    return 0;
}

int pwm_hal_deinit(void) {
    hal_clk_disable_assertrst(CLK_PWM);
    hal_clk_disable(CLK_PWM);
    return 0;
}
