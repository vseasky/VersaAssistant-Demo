---
# 这是文章的标题
title: VersaAssistant
# 这是页面的图标
icon: bookmarks_line
# 这是侧边栏的顺序
order: 2
# 如果你不希望该列表包含一些特定的文章
article: false
# 设置作者
author: vSeasky
# 一个页面可以有多个分类
category:
  - VersaAssistant
# 一个页面可以有多个标签
tag:
  - VersaAssistant 
---

# VersaAssistant 调试助手

## 物理接口

- 串口通信（支持传统串口和USB虚拟串口）
- 网络调试
    - Tcp Server（支持连接多个客户端）
    - Tcp Client
    - Udp Socket
- USB调试（USB-HID）

::: tip 注意

- 部分USB虚拟串口的物理设备可能没有较好的处理数据发送和接收包长，在单次发送大于64、256等字节数据时，可能存在数据丢失问题，这个属于设备端问题，请优先排查设备端。
- 使用USB-HID设备时，需要考虑数据包长为64、256，来对协议数据的数据长度进行限制。

:::

## 编码格式

理论上支持以下编码格式。

| 列1               | 列2              | 列3               | 列4           | 列5                 | 列6           | 列7                  | 列8           |
|------------------|-----------------|------------------|--------------|--------------------|--------------|---------------------|--------------|
| UTF-8            | ISO-8859-1      | latin1           | CP819        | IBM819             | iso-ir-100   | csISOLatin1         | ISO-8859-15  |
| latin9           | UTF-32LE        | UTF-32BE         | UTF-32       | UTF-16LE           | UTF-16BE     | UTF-16              | System       |
| Big5-HKSCS       | Big5            | Big5-ETen        | CP950        | windows-949        | CP949        | EUC-KR              | Shift_JIS    |
| SJIS             | MS_Kanji        | ISO-2022-JP      | JIS7         | EUC-JP             | GB2312       | GBK                 | CP936        |
| MS936            | windows-936     | GB18030          | hp-roman8    | roman8             | csHPRoman8   | TIS-620             | ISO 8859-11  |
| WINSAMI2         | WS2             | macintosh        | Apple Roman  | MacRoman           | windows-1258 | CP1258              | windows-1257 |
| CP1257           | windows-1256    | CP1256           | windows-1255 | CP1255             | windows-1254 | CP1254              | windows-1253 |
| CP1253           | windows-1252    | CP1252           | windows-1251 | CP1251             | windows-1250 | CP1250              | IBM866       |
| CP866            | csIBM866        | IBM874           | CP874        | IBM850             | CP850        | csPC850Multilingual | ISO-8859-16  |
| iso-ir-226       | latin10         | ISO-8859-14      | iso-ir-199   | latin8             | iso-celtic   | ISO-8859-13         | ISO-8859-10  |
| iso-ir-157       | latin6          | ISO-8859-10:1992 | csISOLatin6  | ISO-8859-9         | iso-ir-148   | latin5              | csISOLatin5  |
| ISO-8859-8       | ISO-8859-8-I    | iso-ir-138       | hebrew       | csISOLatinHebrew   | ISO-8859-7   | ECMA-118            | greek        |
| iso-ir-126       | csISOLatinGreek | ISO-8859-6       | ISO-8859-6-I | ECMA-114           | ASMO-708     | arabic              | iso-ir-127   |
| csISOLatinArabic | ISO-8859-5      | cyrillic         | iso-ir-144   | csISOLatinCyrillic | ISO-8859-4   | latin4              | iso-ir-110   |
| csISOLatin4      | ISO-8859-3      | latin3           | iso-ir-109   | csISOLatin3        | ISO-8859-2   | latin2              | iso-ir-101   |
| csISOLatin2      | KOI8-U          | KOI8-RU          | KOI8-R       | csKOI8R            | iscii-mlm    | iscii-knd           | iscii-tlg    |
| iscii-tml        | iscii-ori       | iscii-gjr        | iscii-pnj    | iscii-bng          | iscii-dev    | TSCII               |

## 通信协议

- 仅支持本软件的通信协议，不支持 `print` 这种低效的协议.
- 协议显示窗口，可进行简单数据处理，将数据转换到一定范围，进行数据波动观察

## 波形显示

- 支持基于通信协议的波形显示功能，最大支持28条波形显示。
- 波形绘制横坐标单位为秒。

::: danger

- 有波形刷新时不要进行波形数据导入和波形数据导出操作，如需操作，需要先波形刷新，即关闭通信，或关闭协议模式。
- 波形数据导入前应当先清空波形。
  :::

## 模型加载

- 支持加载自定义3D模型。
- 支持模型的六轴姿态显示。
- 支持机械臂的姿态解算，进行机械臂3D姿态显示。
- 设置模型后需要重启操作。

```json
{
  "ModelPath": [
    "E:/projects/VersaAssistant/VersaAssistant/models/DJIMatrice30T.obj",
    "E:/projects/VersaAssistant/VersaAssistant/models/dummy.obj"
  ],
  "VersaRobot": {
    "DJIMatrice30T.obj": {
      "Matrice30T": {
        "parent": "DJIMatrice30T.obj",
        "action": "transform"
      }
    },
    "dummy.obj": {
      "robot-node1": {
        "parent": "dummy.obj",
        "action": "transform"
      },
      "robot-node2": {
        "parent": "robot-node1",
        "action": "rotation",
        "rotation_coordinates": [
          0.0,
          0.0835,
          0.0
        ],
        "rotation_axis_enable": [
          0.0,
          1.0,
          0.0
        ],
        "rotation_limit_angle": [
          -180.0,
          180.0
        ]
      },
      "robot-node3": {
        "parent": "robot-node2",
        "action": "rotation",
        "rotation_coordinates": [
          0.035,
          0.109,
          0.0
        ],
        "rotation_axis_enable": [
          0.0,
          0.0,
          1.0
        ],
        "rotation_limit_angle": [
          -180.0,
          180.0
        ]
      },
      "robot-node4": {
        "parent": "robot-node3",
        "action": "rotation",
        "rotation_coordinates": [
          0.035,
          0.255,
          0.0
        ],
        "rotation_axis_enable": [
          0.0,
          0.0,
          1.0
        ],
        "rotation_limit_angle": [
          -180.0,
          180.0
        ]
      },
      "robot-node5": {
        "parent": "robot-node4",
        "action": "rotation",
        "rotation_coordinates": [
          0.023,
          0.307,
          0.0
        ],
        "rotation_axis_enable": [
          1.0,
          0.0,
          0.0
        ],
        "rotation_limit_angle": [
          -180.0,
          180.0
        ]
      },
      "robot-node6": {
        "parent": "robot-node5",
        "action": "rotation",
        "rotation_coordinates": [
          0.150,
          0.307,
          0.0
        ],
        "rotation_axis_enable": [
          0.0,
          0.0,
          1.0
        ],
        "rotation_limit_angle": [
          -180.0,
          180.0
        ]
      }
    }
  }
}
```

### 参数说明

#### 模型加载配置

| 参数         | 用途                 |
|------------|--------------------|
| ModelPath  | 模型路径（支持多个模型，obj格式） |
| VersaRobot | 模型动作配置             |

#### 动作配置

VersaRobot 属性下列举模型的文件名例如 `DJIMatrice30T.obj` 和 `dummy.obj`。

- `DJIMatrice30T.obj` 模型中只有一个节点 `Matrice30T`，在此配置其父节点（子节点需要相对于父节点进行姿态变换，如果不设置则是相对于原点变换），设置动作为 `transform` 表示支持所有运动变换，旋转和移动、一共六个动作。
- `dummy.obj` 中有六个节点、在此需要每个节点相对于前一个节点进行姿态变换，同时节点一是相对于原点变换的，因此设置为 `transform` 以支持旋转和移动、一共六个动作。
- 动作`action`设置为`rotation`，表示旋转，旋转需要一共附带的参数，即相对于原点的位置，"rotation_coordinates": `[x,y,z]` 和旋转角度 "rotation_limit_angle":`[agnle1,agnle2]`。
- 如果绕 x 轴旋转，其中 `rotation_coordinates` 的设置中 y和z的值必须准确，因为姿态求解过程中，在矩阵中乘积其实是移动回原点后旋转，然后再将旋转后的模型移动到原来的位置。
- `rotation_limit_angle` 参数需要设置，但是无任何作用，为保留项，此软件并没有使用json中的角度进行限制，而是限制的直接限制的 `[-180，180]`。

::: tip
如果你需要调试机械臂 `rotation_coordinates` 的正确设置是很重要的，在此你可以参考稚辉君的 `dummy` 模型。
:::

此模型为 `Blender` 导出的 `obj` 模型,其它平台，并未测试，如果不兼容，可以导入 `Blender`后导出，不建议使用层级较为复杂的模型，如果节点层级层级复杂，可以在 `Blender` 中合并部分部节点，以仅保留需要进行控制的整体节点，如 `dummy.obj`的节点，你可以在 `Blender` 中查看。

![Snipaste_2023-10-31_13-26-34.png](https://www.liuwei.pub/pic/2023/10/31/6540902a404dd.png)

![Snipaste_2023-10-31_13-27-58.png](https://www.liuwei.pub/pic/2023/10/31/65409063bdf77.png)

导出

![Snipaste_2023-10-31_13-31-40.png](https://www.liuwei.pub/pic/2023/10/31/6540915db58fe.png)


::: details 变换过程

坐标变换的过程

```cpp
/*!
 * 姿态求解，更新为四元数
 * @param pNode
 */
void VersaOpenGLWidget::updataTranslateMatrix(VersaNode *pNode)
{
	// 平移矩阵
	pNode->mTranslateMatrix.setToIdentity();
	pNode->mTranslateMatrix.translate(pNode->mTranslate3D);
	
	QQuaternion pitch, yaw, roll;
	
	// 绕轴旋转矩阵
	if (pNode->mRotation3DAxisEnable.x() != 0)
	{
		pitch = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0),
		                                      pNode->mRotation3DAngle.x());
	}
	if (pNode->mRotation3DAxisEnable.y() != 0)
	{
		yaw = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0),
		                                    pNode->mRotation3DAngle.y());
	}
	if (pNode->mRotation3DAxisEnable.z() != 0)
	{
		roll = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1),
		                                     pNode->mRotation3DAngle.z());
	}
	
	QQuaternion rotation = yaw * pitch * roll;
	
	// 旋转矩阵
	pNode->mRotationMatrix.setToIdentity();
	pNode->mRotationMatrix.translate(pNode->mRotation3DCoordinates.x(),
	                                 pNode->mRotation3DCoordinates.y(),
	                                 pNode->mRotation3DCoordinates.z());
	pNode->mRotationMatrix.rotate(rotation);
	pNode->mRotationMatrix.translate(-pNode->mRotation3DCoordinates.x(),
	                                 -pNode->mRotation3DCoordinates.y(),
	                                 -pNode->mRotation3DCoordinates.z());
	
	// 缩放矩阵
	pNode->mScaleMatrix.setToIdentity();
	pNode->mScaleMatrix.scale(pNode->mScale);
	
	// 相对位置矩阵，控制中不操作，为模型加载时的相对位置
	// mRelativeMatrix
	
	// 最终的变换矩阵
	pNode->mTransformMatrix =
			pNode->mTranslateMatrix *
			pNode->mRotationMatrix *
			pNode->mScaleMatrix *
			pNode->mRelativeMatrix;
}
```

模型绘制过程

```cpp
void VersaOpenGLWidget::drawNode(const VersaNode *pNode, QMatrix4x4 drawScaleMatrix)
{
    // drawScaleMatrix可以理解为父节点变换矩阵
    
	drawScaleMatrix *= pNode->mTransformMatrix;
	QMatrix4x4 modelViewMatrix;
	QMatrix3x3 normalMatrix;
	QMatrix4x4 mvp;
	
	// 先缩放，再旋转，再平移 。
	modelViewMatrix = mCameraView * drawScaleMatrix;
	normalMatrix = modelViewMatrix.normalMatrix();
	mvp = mProjection * modelViewMatrix;
	mShaderProgram.setUniformValue("MV", modelViewMatrix);
	mShaderProgram.setUniformValue("N", normalMatrix);
	mShaderProgram.setUniformValue("MVP", mvp);
	
	for (int imeshes = 0; imeshes < pNode->mMeshes.size(); ++imeshes)
	{
		if (pNode->mMeshes[imeshes]->material->mName == QString("DefaultMaterial"))
		{
			setMaterialUniforms(&mMaterialInfo);
		}
		else
		{
			setMaterialUniforms(pNode->mMeshes[imeshes]->material.data());
		}
		{
			mVao.bind();
			glDrawElements(GL_TRIANGLES,
			               pNode->mMeshes[imeshes]->mIndexCount,
			               GL_UNSIGNED_INT,
			               (const void *) (pNode->mMeshes[imeshes]->mIndexOffset * sizeof(unsigned int)));
			mVao.release();
		}
	}
	// 递归绘制该节点的子节点
	for (int inn = 0; inn < pNode->mNodes.size(); ++inn)
	{
		drawNode(&pNode->mNodes[inn], drawScaleMatrix);
	}
}
```

:::

## 主题颜色

```json
{
  "theme-color": [
    "#FFFFFF",
    "#0066FF",
    "#29A9FF",
    "#91C6FF",
    "#118DF0",
    "#C70039",
    "#FFC773"
  ]
}
```

依次为背景色、标题栏颜色、侧边栏颜色、滑块颜色、按钮悬停颜色、按钮按下颜色、菜单选项选中颜色。

## 支持保存工作区布局

- 可以将工作区布局保存为单个文件，同时可以将部分数据和工作区一起保存。
- 可以选择配置文件以恢复工作区。

## 性能

- 常规调试界面大量数据不会卡顿。
- 实测 `2000000` 波特率串口，每 `ms` 发送 28 个 `float` 数据,即 `28*4+16`=`256` 字节可以较长正常工作。
- 无法保证所有电脑上都能正常运行，大量的波形绘制可能需要您的设备有较好的性能，较低的配置可能导致卡死，如遇此问题，可以设置较小的数据量。