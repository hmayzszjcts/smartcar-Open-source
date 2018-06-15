
#if 0
case PLL50:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(24);   //25分频 ：2M
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// Set system options dividers
//MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
set_sys_dividers(0, 0, 0, 1);
// Set the VCO divider and enable the PLL for 50MHz,, LOLIE=0, PLLS=1, CME=0, VDIV=1
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
pll_freq = 50;      //50/25*25=50
break;

case PLL96:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(24);   //25分频 ：2M
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// Set system options dividers
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
pll_freq = 96;          //50/25*48=
break;

case PLL100:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(19);   //20分频 ：2.5M
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(16); //VDIV = 16 (x40)
pll_freq = 100;         //50/20*40
break;

case PLL120:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(14);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(12);
pll_freq = 120;
break;
case PLL125:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(13);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(11);
pll_freq = 125;
break;

case PLL130:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(14);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(15);
pll_freq = 130;
break;

case PLL140:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(14);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 1, 1, 3);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(18);
pll_freq = 140;
break;

case PLL144:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(16);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
//set_sys_dividers(0,1,1,3);
set_sys_dividers(0, 2, 2, 4);
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(25);
pll_freq = 144;
break;

case PLL150:
//进入FBE模式
MCG_C5 = MCG_C5_PRDIV(17);
MCG_C6 = 0x0;          // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear

// 设置系统分频器
//set_sys_dividers(0,1,1,3); //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
set_sys_dividers(0, 2, 2, 5); //core=150M, bus=50M, FlexBus=50M, Flash Clk=25M
// Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(30);
pll_freq = 150;
break;

#endif

