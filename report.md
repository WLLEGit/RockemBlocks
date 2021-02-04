# RockemBlocks Report

## 1、使用指南

### 1）游戏规则

1. 一块方形区域被平均分成各种颜色不同的色块，每点击一个宝石，它及它周围的同色宝石被消除。
2. **特殊宝石**（外观黑色有旋转特效）点击后，引发全场与其正上方相邻色块颜色相同的宝石爆炸。
3. 每局**三分钟**，记分模式，有**一次重置全场宝石**的机会。一次行动后若无可消去宝石，会触发重置（如果有有剩余机会）。
4. 记分规则：普通宝石消灭一个获得5分。L型、T型、十字形消除，分别另外获得15，20，25分（边长需大于等于2）。
5. **结束**规则：三分钟时间到或者场上无可消去宝石且无重置机会。

### 2）操作方法

记得打开声音（音量20左右即可），获得完整体验。

**点击程序**后素材加载大约需要5~10秒，然后弹出窗口，别着急。

**第一次启动**程序点击开始界面会提示输入用户名，之后程序启动会默认使用上次登录的用户名。

**更改用户名**可以点击左下角的“xxxx欢迎回来”字样，弹出改名界面。若不输入点击确定则不作修改。

左边**记录**按钮从高到低展示历次分数。右边**设置**按钮选择难度，决定场上宝石种类数量。右下角退出按钮。

记录、设置、菜单**子界面点击背景灰黑色区域退出**。结算界面点击确定返回主界面。

**进入游戏后**左上角显示当前分数，左下角菜单可以返回主界面。下方进度条共三分钟。重置键有一次机会，使用后（包括无可消去宝石时的自动重置）变为灰色。

## 2、概况

![界面树形图](D:\OneDrive\文档\桌面\Snap\界面树形图.png)

### 0）一共定义了7个类：

1. **两个主窗口类**StartWidget和GameWidget
2. **两个子界面类**Dialog和SettingWidget
3. **三个封装的按钮**MenuButton（实现开始界面图案的旋转效果）, HoverButton（可以在鼠标经过时改变图片和音效）和Gem（游戏中的宝石）

### 1）开始界面(StartWidget类)

#### (I) 界面

------

<img src="D:\OneDrive\文档\桌面\Snap\主界面.png" alt="主界面" style="zoom: 33%;" /> 程序入口界面(StartWidget类)

**背景**为QLabel。**动画效果**来自QPropertyAnimation。

**按钮**：中间三个为MenuButton类， 左下角为封装的QTextEdit，右下角为HoverButton类

------

<img src="D:\OneDrive\文档\桌面\Snap\排行榜界面.png" alt="排行榜界面" style="zoom:33%;" />排行榜界面(recordBoard)

**黑色背景**为QPushButton，设置Icon为半透明黑色。为了点击时返回所以用了QPushButton（下同）

**排行榜背景**为Label设置图片

里面放上QLabel（顶端）和QTabelWidget（中间）

------

<img src="D:\OneDrive\文档\桌面\Snap\设置界面.png" alt="设置界面" style="zoom:33%;" />设置界面(SettingWidget类 type1)

**黑色背景**QPushButton，**背景**QLabel，**中间构成**QLabel+QPushButton*4

------

<img src="D:\OneDrive\文档\桌面\Snap\设置用户名界面.png" alt="设置用户名界面" style="zoom:33%;" />设置用户名界面(SetUser()函数)

**黑色背景**QPushButton，**背景**QLabel，**中间**QLabel*2 + QLineEdit + HoverButton

------

#### (II) 音效

**背景音乐**：Loading.mp3, MainMenu.mp3

**按钮音效**：button_mouseover.wav（掠过）, button_mouseleave.wav（离开）,  button_press.wav（按下）, button_release.wav（释放）

### 2）游戏界面

#### (I) 界面

------

<img src="D:\OneDrive\文档\桌面\Snap\游戏界面.png" alt="游戏界面" style="zoom:33%;" />游戏界面(GameWidget类)

背景QLabel， 左下角QLabel+HoverButton*2， 右下角进度条

------

<img src="D:\OneDrive\文档\桌面\Snap\菜单界面.png" alt="菜单界面" style="zoom:33%;" />菜单界面(SettingWidget类 type2)

------

<img src="D:\OneDrive\文档\桌面\Snap\结算界面.png" alt="结算界面" style="zoom:33%;" />结算界面(settlementDialog)

------

#### (II) 音效

**背景音乐**：Classic.mp3

**音效**：badmove.wav（无法消去）, combo_2.wav（消去）, gem_hit.wav（宝石下落）, gem_shatters.wav（特殊宝石）, voice_awesome.wav, voice_excellent.wav, voice_go.wav, voice_good.wav, voice_nomoremoves.wav（没有可消去的宝石）, voice_timeup.wav（时间到）, voice_unbelievable.wav

### 3）数据存储方式

数据存储在同目录下的**record文件**（无后缀名）中。存储样例为：

```Text
UserName				//第一行为上次运行时的用户名
UserName1 Score1		//用户名+空格+分数+回车，降序排列
UserName2 Score2
UserNmae3 Score3
						//最后一行空行
```

数据字符编码为UTF-8无签名，程序读取时按照此编码读写，更改为其他编码可能乱码。

## 3、设计思路与代码实现

如果需要编译代码的话，记得放到纯英文路径下，并且更改gamewidget.cpp和startwidget.cpp下的两处QMediaPlayer背景音乐内的音频路径（因为QMediaPlayer不支持Qt资源文件，必须要使用绝对路径），否则会出现Q_Assert报错。

### 1）设计思路

#### (0) 写在前面

起初选择消消乐这个目标的时候觉得游戏逻辑似乎挺清晰的，没想到控制台编程和Qt编程之间竟然有这么大的差距，游戏逻辑与鼠标操作的融合费了一番功夫。尽管在此之前学习了Qt的相关知识，但在这次项目的编写过程中仍然遇到了很多意料之外的bug，最终查阅了很多文档博客和解决了这些问题。这个项目起初不准备做这么大，但是越做越想将它做得更完善些，在此期间学习了Qt的很多基本控件以及包括QPropertyAnimation、QFile、QTextStream、QMediaPlayer、QSound在内的很多教程中没有教的内容，同时在解决bug的过程中对Qt特性的把握更加深刻，我想这是我这个项目最大的收获。

这次的媒体素材主要来自知名游戏《宝石迷阵》，当然没有现成的，音效背景音乐都是我找了一个apk之后解包得来，图片素材由于原文件的分散性，还进行了图像处理（比如动图其实是一张大图拆成40张独立图片再用Python合成的），顺便锻炼了Ps和Python能力（x。

下面的思路介绍大致以时间顺序，这次的代码的编写过程全程体现在GitHub上(https://github.com/WLLEGit/RockemBlocks)。$_{（目前还是私有状态）}$

#### (I) 第一阶段：构思

首先宏观上确定了程序的框架：主界面+游戏界面。主界面内有三大按键：记录、开始、设置，右下角有退出按键，左下角显示用户；游戏界面主要为计分板、功能键和游戏区域。点击按键跳出相应界面，游戏结束跳出得分界面。

#### (II) 第二阶段：主界面主要功能的实现

这一阶段实现了主界面的显示，主要影响startwidget.cpp, hoverbutton.cpp, menubutton.cpp。

**程序点开的动画效果**是通过背景图片图片从下往上滚动，按钮从下向上出现实现的。体现在代码中为ShowBackground和ShowButton两个函数。同时播放背景音乐，设置循环播放（将停止事件和播放事件链接）。

为了实现中间三个按钮的gif显示，封装了**MenuButton类**，以一个HoverButton覆盖QLabel，HoverButton负责显示文字，QLabel负责显示动图。HoverButton在鼠标悬浮时字体放大，不同的鼠标事件有不同的音效。按钮的位置经过多次调整之后自我感觉较为合适。

**HoverButton的实现**也值得一说，原生的QPushButton不支持hover事件（鼠标悬浮），于是需要重写event事件，在hover、leave，press、release四个事件发生时切换图片、发出声音。接口为setImage和setSound。隐去button矩形样子是通过`setStyleSheet("QPushButton{border:0px;}")`实现的。

接着将四个按钮和对应的槽函数**用connect连接**，未实现的先用空函数代替。实现**过场动画**TransitionAnimation函数，其间为了延时并且不影响其他代码执行，利用博客上`            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);`实现。

**start按钮点击**时，停止当前背景音乐，播放游戏画面背景音乐，（游戏界面的指针由主界面维护），播放切换时的过场动画（中心按钮放大，窗口减淡消失）。

#### (III) 第三阶段：配置游戏场景

这一阶段实现游戏场景的显示，包括各种图案和可以点击的宝石，但并不包括游戏逻辑。

**背景图片**用一张QLabel显示。**棋盘**的浅灰深灰（其实是不同透明度的纯黑）相间是通过QPainter实现的，其中有个坑点是Qpainter设置透明度会影响所有painter，这导致了不能同时使用两个不同透明度的画笔来画，只能画完一个，再画一个。

左边的**记分牌**和**功能区边框图案**、棋盘上下装饰用Label显示，左上方有个label显示分数，左下方放上两个HoverButton显示**重置和菜单键**，大小和位置是通过和外边框图案的相对位置测算出来的（拿Ps量的）。遇到一个bug是因为Label显示图片会截断大的部分导致显示不全，要想达到理想效果得用border-image来设置。connect相应按键和槽函数（未实现）。

右下角进度条为QProgressBar设置了StyleSheet，总长3分钟。

棋盘是关键，我选择采用一个**QWidget作为棋盘**，分割为10×10，每个格子放入一个**Gem类**变量作为宝石。用两个二维数组`gems、gemBoard`维护宝石指针和宝石种类。Gem类对于普通宝石只是QPushButton换个皮，同时按下时释放信号传递自身指针（为了后来消去逻辑的实现，有个坑点下个阶段再说）。对于特殊宝石还需要贴上一个QLabel来显示gif效果$_{（为了显得与众不同）}$。

#### (IV) 第四阶段：游戏逻辑的实现

一开始以为思路很清晰，由于只有10*10不用考虑TLE的问题，只要从当前宝石坐标开始用`GemBomb`递归向外查找，看看一共有多少宝石连在一起即可（用toBomb的vector来存储将要消去的宝石）。直到后来我遇到了**bug**。

第一个是方形相连会导致死循环，找到后成功解决。第二个是Qt特性的原因，`connect(gems[i][j], &Gem::clicked, [=][(){balabala...;act(gems[i][j]);}])`，connect的槽函数写成lambda函数时里面的`gems[i][j]`指针不是一个具体的指针，而是变化的，哪个宝石在i j位置是哪个，而不是预期中的创建时的i，j。这个bug找了很久才解决，也就是Gem加一个信号来告诉gamewidget谁发出了信号。

为了实现不同形状的消去加分，返回值设置为了BombInfo，附加了是否为直线的值。**act函数**中销毁toBomb中的宝石并记录分数，在销毁宝石前根据toBomb数组更新toFall数组，toFall数组中ij位置表示ij宝石需要下落的格数，已被销毁的记为-1。

接着实现**填补棋盘空缺**的fillBoard函数，根据toFall数组得到每一列需要填补的空位数，并随机生成宝石，放在棋盘上方（看不见），然后执行fall函数，将宝石落在指定位置。

**fall函数**中使用了一个动画函数`fallAnimation`，宝石`gem`下落`h`个格数。更新`gems、gemBoard、gems变量坐标`三个值。

到这里基本上游戏能玩了，但为了防止疯狂点击导致的动画未执行完就执行下一个动画导致移位，定义了**is_acting变量**来维护是否在执行动画，如果是就**禁用宝石的鼠标事件**。

顺便实现了**重置功能**，复用了initScene函数，重置时有个全场宝石左右震动一下的小效果。除此之外还实现了进度条和计时器。

#### (V) 第五阶段：子界面的加入

说起来游戏制作的主要部分已经完成了，但是想想还有还几个界面没有设计还是比较头疼的。后来五化三，设计两个模板来显示其中四种界面。分别是**SettingWidget类对应设置界面和菜单界面**；**Dialog类对应设置用户名和结算界面**；**排行榜界面**没有单独成类，放在startWidget里面。

两个类的实现上主要时间花在了调整位置上，背景设计为一个button而不是label是为了点击可以做出返回操作，难度设定按钮按下调用setDifficulity函数，里面负责改变难度、重设棋盘、改变图标等工作。

排行榜recordBoard、结算界面、用户名界面还需要负责record文件的读写，为了少些一点`std::`之类的东西，选择了**QFile和QTextStream**而不是fstream。但问题随之而来，中文的读写成了问题，中文会导致乱码，查找资料后发现是QTextStream默认编码问题，需要加上`setCodeC("utf-8");`（每个都要设置，期间出现过打成uft-8找不到问题在哪的bug。对于不可编辑的排行榜和结算界面，需要设置`setReadOnly(true);`。record界面每次加入数据时需要进行**排序**，由于QFile的插入行并不好实现并且考虑到数据量很小，采用了全部读取，快排后覆盖写入的方式进行。

当子界面加入、页面元素增多，会出现控件**上下关系错乱**的问题。原因是**后new的控件总是显示在先new控件的上面**，道理谁都懂，但写起代码经常注意不到这点，特别是重置棋盘后棋盘显示在菜单和结算界面上面。对此的解决方案是在每个子控件`show()`之前**调用`raise()`方法将它放到顶层**。

另外，**触发式创建的控件和widget默认是不显示的**，所以会出现创建了控件显示不出来的问题，需要调用`show()`方法来显示。

#### (VI) 测试优化

至此游戏已经完成，最后的阶段对游戏进行测试，完善音效、调一调出现概率、找一找bug之类。

Qt的软件发布可以使用自带的windeployqt.exe来实现，成功得到release版本后项目到达终点🎉。

