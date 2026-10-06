# AGENTS.md — HWTask1(电控第一次作业)

## 项目概览

- **芯片**:STM32F103C8T6(F103 最小系统板 / Blue Pill)。换板时对应型号:
  - RoboMaster C 板 = STM32F407IG
  - RoboMaster A 板 = STM32F427II
- **技术栈**:STM32CubeMX(HAL)生成代码 + CMake + Ninja + `arm-none-eabi-gcc`,VS Code 开发,Ozone 调试。
- **三题共用同一个 CubeMX 工程**,业务代码只写在 `Tasks/`。

## 工具链与构建

- 用发放的 `CMakeLists.txt` 替换 CubeMX 生成的那一份,`user_folders` 保持 `"Tasks"`。
- CubeMX:`Toolchain/IDE` 选 **CMake**,勾选 **Generate Under Root**。
- VS Code 构建类型选 **Debug**。
- 命令:
  ```bash
  cmake --preset Debug
  cmake --build --preset Debug
  ```
- 产物:`build/` 下的 `.elf`(用 Ozone 打开),`.hex`/`.bin` 在 `build/Debug/`。
- ⚠️ 当前工程名还是 `project`(`project(project ...)`),可改成有意义的名字,产物文件名会跟着变。

## 目录与代码约定

- `Tasks/src/` 放 `.c`/`.cpp`,`Tasks/inc/` 放头文件;**先建好这两个目录再 Configure**,否则 Tasks 不进编译。
- `main.c` 只在 `USER CODE` 区块里 include 头文件并调用初始化;保持模板 `while(1)` 为空(或只放喂狗/自增逻辑)。
- 自己的代码放 `Tasks` 和 `USER CODE`,CubeMX 重新生成不会冲掉;若 `CMakeLists.txt` 被覆盖,再换回模板。

## HW Task 1 三题要求

### 题 1 — GPIO
- CubeMX 把 LED 脚配成**推挽输出**,在 Tasks 的初始化里写电平点亮。
- 引脚与点亮电平:
  - F103:`PC13` 低电平亮(板载灯)
  - C 板:`PH11` 绿灯亮
  - A 板:`PF14` 绿灯亮
- **不要在主循环里翻转。**

### 题 2 — 定时器
- 启用一个定时器的**更新中断**,周期 **1 ms**。
- 全局 `volatile uint32_t tick`,在回调里 `tick++` 并调用 `HAL_IWDG_Refresh(&hiwdg)`。
- 周期计算:`(PSC + 1) * (ARR + 1) / 定时器时钟 = 0.001 s`。
  - 定时器挂 APB1 且 APB1 分频 ≠ 1 时,定时器时钟 = APB1 × 2,以 CubeMX 时钟树为准。
- **不要用 `HAL_Delay` 或空循环加 tick。**

### 题 3 — 看门狗
- IWDG 全程启用,超时约 **2 s**(明显长于 1 ms)。
- 超时公式:`超时 = (Reload + 1) × 分频 / LSI`;F103 LSI ≈ 40 kHz,C/A 板 ≈ 32 kHz。
- 参考配置(可直接用):

  | 板子 | 分频 | Reload | 超时 |
  |------|------|--------|------|
  | F103 | 64 | 1249 | 2 s |
  | C / A 板 | 64 | 999 | 2 s |
- 题 3 **去掉回调里的 `HAL_IWDG_Refresh`**,重新编译下载。
- 预期:`tick` 从 0 涨到约 2000 后归零,反复(复位后启动代码清掉全局变量)。

## 关键坑(重要)

- IWDG **从题 1 起就启用**,三题都不关。题 1 还没喂狗,暂停后约 2 s 内看完,否则芯片复位、调试器掉线。
- 定时器更新回调**全工程只写一份**,放 Tasks 源文件;CubeMX 若在 `main.c` 或中断文件生成同名函数,删掉那一份,否则链接报重复定义。
- 用 C++ 写回调时,函数要加 **`extern "C"`**。
- `tick` 必须定义成**全局 `volatile uint32_t`**,否则 Ozone 看不到 / 被优化掉。
- Ozone 观察时**让程序跑着,不要停在断点**;CPU 停住时看门狗仍在计数,调试器会掉线。

## 提交要求

- **截止**:2026-10-06 22:00。
- 邮件主题:`姓名_学号_电控第1次作业`;发到 `2372@qq.com`、`13567781025@163.com`,抄送 `207782`。
- 建 **GitHub 公开仓库**,代码推上去,邮件正文写仓库链接。
- 截图/照片全部放**附录**(只放附录,不插正文):
  1. GPIO:灯亮照片;
  2. 定时器:`tick` 的 Ozone 监视/Timeline,能看出约每秒增加 1000;
  3. 看门狗:`tick` 的 Timeline/连续监视,能看到涨上去再归零。
- 建议/问题写成 **PDF**,随邮件附件。
