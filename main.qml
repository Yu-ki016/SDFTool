import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 600
    height: 400
    visible: true
    title: qsTr("SDF工具")

    property string sdfPath: "在这里输入SDF路径"
    property string atlasPath: "在这里输入图集路径"

    Column {
        id: column
        anchors.fill: parent
        transformOrigin: Item.Center

        Item {
            id: row
            height: 32
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0

            Text {
                id: text_Path1
                text: qsTr("SDF路径:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 10
                transformOrigin: Item.Center
                anchors.bottomMargin: 0
                anchors.topMargin: 0
            }

            Rectangle {
                id: rectangle
                width: 440
                color: "#ffffff"
                border.width: 1
                anchors.left: text_Path1.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                anchors.leftMargin: 10
                layer.enabled: false
                transformOrigin: Item.Center

                TextEdit {
                    id: text_SDF_Path
                    x: 49
                    width: 240
                    height: 32
                    text: sdfPath
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.top: parent.verticalCenter
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: -16
                    transformOrigin: Item.Center
                    bottomPadding: 0
                    leftPadding: 3
                    topPadding: 0

                }
            }

            Button {
                id: button
                width: 60
                text: qsTr("选择路径")
                anchors.left: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                transformOrigin: Item.Center
                anchors.leftMargin: -70
                autoExclusive: true
                highlighted: false
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                onClicked: {
                    console.log("Botton Clicked")
                    sdfPath = pyFunc.selectPath()
                }
            }


        }

        Item {
            id: row1
            height: 32
            anchors.left: parent.horizontalCenter
            anchors.right: parent.horizontalCenter
            anchors.top: row.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 0

            Button {
                id: button1
                width: 60
                height: 32
                text: qsTr("生成SDF")
                anchors.left: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.leftMargin: -30
                onClicked: {
                    console.log("Botton Clicked")
                    pyFunc.generateSDF(sdfPath)
                }
            }
        }

        Item {
            id: row2
            width: column.width
            height: 32
            anchors.top: row1.bottom
            anchors.topMargin: 0

            Text {
                id: text_Path2
                width: 50
                text: qsTr("图集路径:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 10
                transformOrigin: Item.Center
                anchors.bottomMargin: 0
                anchors.topMargin: 0
            }

            Rectangle {
                id: rectangle2
                width: 440
                color: "#ffffff"
                border.width: 1
                anchors.left: text_Path2.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                anchors.leftMargin: 10
                layer.enabled: false
                transformOrigin: Item.Center

                TextEdit {
                    id: txt_path2
                    x: 49
                    width: 240
                    height: 32
                    text: atlasPath
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.top: parent.verticalCenter
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: -16
                    transformOrigin: Item.Center
                    bottomPadding: 0
                    leftPadding: 3
                    topPadding: 0

                }
            }

            Button {
                id: button2
                width: 60
                text: qsTr("选择路径")
                anchors.left: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                transformOrigin: Item.Center
                anchors.leftMargin: -70
                autoExclusive: true
                highlighted: false
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                onClicked: {
                    console.log("Botton Clicked")
                    atlasPath = pyFunc.selectPath()
                }
            }


        }

        Item {
            id: row3
            width: column.width
            height: 32
            anchors.top: row2.bottom
            anchors.topMargin: 0

            CheckBox {
                id: isTopDownOneTexture
                width: 160
                height: row3.height
                text: qsTr("首行和末行只有一张图片")
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                checkState: Qt.Unchecked
                tristate: false
                anchors.leftMargin: 0
            }

            Item {
                id: item1
                y: 0
                width: 190
                height: row3.height
                anchors.left: parent.left
                anchors.leftMargin: 150

                Text {
                    id: text_RowAndCols
                    x: 10
                    y: 0
                    width: 75
                    text: qsTr("行数x列数:")
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 0
                    transformOrigin: Item.Center
                    anchors.bottomMargin: 0
                    anchors.topMargin: 0
                }

                SpinBox {
                    id: int_row
                    width: 45
                    value: 9
                    height: row3.height - 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: text_RowAndCols.right
                    anchors.leftMargin: 0

                }

                Text {
                    id: text_x
                    width: 20
                    text: qsTr("x")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: text_RowAndCols.right
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: int_row.width
                    transformOrigin: Item.Center
                }

                SpinBox {
                    id: int_col
                    width: 45
                    value: 9
                    height: row3.height - 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: text_x.right
                    anchors.leftMargin: 0

                }
            }

            Item {
                id: item2
                width: 160
                height: row3.height
                anchors.left: item1.right
                anchors.leftMargin: 0

                Text {
                    id: text_Resolution
                    x: 10
                    y: 0
                    width: 90
                    text: qsTr("图集最终分辨率:")
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 0
                    transformOrigin: Item.Center
                    anchors.bottomMargin: 0
                    anchors.topMargin: 0
                }

                SpinBox {
                    id: int_Resolution_X
                    width: 70
                    value: 4096
                    height: row3.height - 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: text_Resolution.right
                    stepSize: 512
                    to: 8192
                    anchors.leftMargin: 0
                }

                Text {
                    id: text_x2
                    width: 20
                    text: qsTr("x")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: int_Resolution_X.right
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 0
                    transformOrigin: Item.Center
                }

                SpinBox {
                    id: int_Resolution_Y
                    width: 70
                    value: 4096
                    height: row3.height - 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: text_x2.right
                    stepSize: 512
                    to: 8192
                    anchors.leftMargin: 0
                }
            }

        }

        Item {
            id: row4
            height: 32
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: row3.bottom
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0

            Button {
                id: button3
                width: 60
                height: 32
                text: qsTr("生成图集")
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: -30
                onClicked: {
                    console.log("Botton Clicked")
                    pyFunc.generateAtlas(atlasPath, int_row.value, int_col.value, int_Resolution_X.value, int_Resolution_Y.value,isTopDownOneTexture.checked)
                }
            }
        }



    }
}




