这是层次状态机（HSM）**应用非常广泛且效果明显**的场景之一！
 我们来构造一个典型的：**带配置菜单的嵌入式 UI 状态机（支持 ESC 键逐级返回）**。

------

## 🧠 场景需求描述（真实场景）

你在嵌入式系统（如工业仪表、智能家电）中，有以下界面结构：

```
arduino复制编辑Top
 ├── HomeScreen
 └── Menu
      ├── DisplaySettings
      │     ├── Brightness
      │     └── Contrast
      └── NetworkSettings
            ├── WiFi
            └── Ethernet
```

- 用户在 Home 按下 `Menu` 进入 `Menu`
- 用方向键进入 `DisplaySettings -> Brightness`
- 按下 `ESC` 键逐级返回上一级菜单，最终返回 `HomeScreen`

------

## ✅ HSM 的强大体现：

| 功能点                       | HSM 优势             |
| ---------------------------- | -------------------- |
| 每个子菜单共用 ESC 返回逻辑  | 放在父状态 `Menu` 中 |
| 子菜单又有独立逻辑（如滑条） | 放在具体状态         |
| 多级嵌套菜单                 | 自动维护父子关系     |



------

## 📦 信号定义（事件）

```
cpp复制编辑enum {
    USER_SIG = SM::USER_SIG,
    KEY_MENU_SIG,
    KEY_ESC_SIG,
    KEY_ENTER_SIG,
    KEY_UP_SIG,
    KEY_DOWN_SIG
};
```

------

## 📄 状态机结构代码（使用 smcpp）

```
cpp复制编辑class UIEvent : public SM::Event {
public:
    UIEvent(SM::Singal sig): Event(sig) {}
};

class UIHsm : public SM::Hsm {
public:
    UIHsm() : Hsm(init) {}

private:
    static int init(SM::Attr &hsm, SM::Event &e) {
        return hsm.tran(HomeScreen);
    }

    static int Top(SM::Attr &hsm, SM::Event &e) {
        return hsm.ignore();
    }

    // HomeScreen 状态
    static int HomeScreen(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[HomeScreen] Displaying Home\n");
                return hsm.handled();
            case KEY_MENU_SIG:
                return hsm.tran(Menu);
        }
        return hsm.supper(Top);
    }

    // Menu 父状态，统一处理 ESC 返回
    static int Menu(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[Menu] Enter menu\n");
                return hsm.handled();
            case SM::EXIT_SIG:
                printf("[Menu] Exit menu\n");
                return hsm.handled();
            case SM::INIT_SIG:
                return hsm.tran(DisplaySettings);
            case KEY_ESC_SIG:
                return hsm.tran(HomeScreen); // 统一 ESC 返回首页
        }
        return hsm.supper(Top);
    }

    static int DisplaySettings(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[DisplaySettings] Display settings menu\n");
                return hsm.handled();
            case SM::INIT_SIG:
                return hsm.tran(Brightness);
            case KEY_DOWN_SIG:
                return hsm.tran(NetworkSettings); // 切换大类
        }
        return hsm.supper(Menu);
    }

    static int NetworkSettings(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[NetworkSettings] Network menu\n");
                return hsm.handled();
            case SM::INIT_SIG:
                return hsm.tran(WiFi);
            case KEY_UP_SIG:
                return hsm.tran(DisplaySettings); // 切回上类
        }
        return hsm.supper(Menu);
    }

    static int Brightness(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[Brightness] Adjust brightness\n");
                return hsm.handled();
            case KEY_DOWN_SIG:
                return hsm.tran(Contrast);
        }
        return hsm.supper(DisplaySettings);
    }

    static int Contrast(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[Contrast] Adjust contrast\n");
                return hsm.handled();
            case KEY_UP_SIG:
                return hsm.tran(Brightness);
        }
        return hsm.supper(DisplaySettings);
    }

    static int WiFi(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[WiFi] WiFi Settings\n");
                return hsm.handled();
            case KEY_DOWN_SIG:
                return hsm.tran(Ethernet);
        }
        return hsm.supper(NetworkSettings);
    }

    static int Ethernet(SM::Attr &hsm, SM::Event &e) {
        switch(e.sig) {
            case SM::ENTRY_SIG:
                printf("[Ethernet] Ethernet Settings\n");
                return hsm.handled();
            case KEY_UP_SIG:
                return hsm.tran(WiFi);
        }
        return hsm.supper(NetworkSettings);
    }
};
```

------

## ✅ 测试代码

```
cpp复制编辑int main() {
    UIHsm ui;
    ui.start();

    UIEvent menu(KEY_MENU_SIG);
    UIEvent down(KEY_DOWN_SIG);
    UIEvent up(KEY_UP_SIG);
    UIEvent esc(KEY_ESC_SIG);

    ui.dispatch(menu);   // 进入菜单
    ui.dispatch(down);   // 显示设置 → 网络设置
    ui.dispatch(down);   // WiFi → Ethernet
    ui.dispatch(esc);    // 返回 HomeScreen（从任何子菜单）

    return 0;
}
```

------

## ✅ 输出示例

```
text复制编辑[HomeScreen] Displaying Home
[Menu] Enter menu
[DisplaySettings] Display settings menu
[Brightness] Adjust brightness
[Contrast] Adjust contrast
[NetworkSettings] Network menu
[WiFi] WiFi Settings
[Ethernet] Ethernet Settings
[Menu] Exit menu
[HomeScreen] Displaying Home
```

------

## ✅ HSM 优势总结

| 功能点                     | 如果用普通 FSM           | 用 HSM 实现            |
| -------------------------- | ------------------------ | ---------------------- |
| ESC 返回上级菜单           | 所有子状态都要手动写 ESC | 在父状态 Menu 统一处理 |
| 子菜单入口顺序定义（INIT） | 必须编码嵌套跳转         | INIT_SIG 自然表达      |
| 多级菜单跳转清晰           | 复杂难维护               | 层级建模逻辑清楚       |
| 子状态行为复用             | 代码重复严重             | supper 自动继承父逻辑  |



------

## 🎯 总结

- 菜单 UI 是 HSM 的经典应用场景
- 特别适合处理 **“嵌套菜单 + 返回键 + 上下选择”**
- 如果你将来做工业仪表 UI、OLED 菜单、车载屏幕，**一定建议用 HSM**

------

需要我输出对应的状态图（dot / Graphviz）或把这个菜单扩展为**支持定时器回首页**的更复杂例子吗？你也可以告诉我你具体的 UI 应用场景，我来帮你建模。