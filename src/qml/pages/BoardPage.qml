import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    signal backRequested()

    background: Rectangle {
        color: ThemeManager.colors.welcomeBgColor
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        Rectangle {
            Layout.fillWidth: true
            radius: 22
            color: "#1f1826"
            border.width: 2
            border.color: "#181818"
            implicitHeight: headerLayout.implicitHeight + 20

            ColumnLayout {
                id: headerLayout
                anchors.fill: parent
                anchors.margins: 10
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        text: "Назад"
                        onClicked: root.backRequested()

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 12
                            font.bold: true
                            color: "#141414"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            radius: 16
                            color: "#FFE135"
                            border.width: 1
                            border.color: "#181818"
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        text: AppState.selectedBoardName
                        font.pixelSize: 22
                        font.bold: true
                        color: ThemeManager.colors.textColor
                        elide: Text.ElideRight
                    }

                    Button {
                        text: AppState.loadingBoard ? "Загрузка..." : "Обновить"
                        enabled: !AppState.loadingBoard
                        onClicked: AppState.refreshCurrentBoard()

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 12
                            font.bold: true
                            color: "#141414"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            radius: 16
                            color: "#FFE135"
                            border.width: 1
                            border.color: "#181818"
                        }
                    }
                }
            }
        }

        Rectangle {
            id: viewport
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 24
            color: ThemeManager.colors.backgroundColor
            border.width: 2
            border.color: "#181818"
            clip: true

            property real zoom: 1.0
            property real minZoom: 0.6
            property real maxZoom: 4.0
            property real fitScale: {
                var availableWidth = width - 24
                if (availableWidth <= 0 || AppState.boardCanvasWidth <= 0)
                    return 1.0
                return Math.min(1.0, availableWidth / AppState.boardCanvasWidth)
            }

            Flickable {
                id: boardFlick
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                contentWidth: scene.width
                contentHeight: scene.height
                boundsBehavior: Flickable.StopAtBounds

                Item {
                    id: scene
                    width: Math.max((AppState.boardCanvasWidth * viewport.fitScale * viewport.zoom) + 24, viewport.width - 20)
                    height: Math.max((AppState.boardCanvasHeight * viewport.fitScale * viewport.zoom) + 24, viewport.height - 20)

                    Canvas {
                        id: gridCanvas
                        anchors.fill: parent
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.setTransform(1, 0, 0, 1, 0, 0)
                            ctx.clearRect(0, 0, width, height)
                            ctx.fillStyle = ThemeManager.colors.backgroundColor
                            ctx.fillRect(0, 0, width, height)
                            ctx.strokeStyle = "#2f2f47"
                            ctx.lineWidth = 1
                            var spacing = Math.max(24, 44 * viewport.zoom)
                            for (var x = 0; x < width; x += spacing) {
                                ctx.beginPath()
                                ctx.moveTo(x + 0.5, 0)
                                ctx.lineTo(x + 0.5, height)
                                ctx.stroke()
                            }
                            for (var y = 0; y < height; y += spacing) {
                                ctx.beginPath()
                                ctx.moveTo(0, y + 0.5)
                                ctx.lineTo(width, y + 0.5)
                                ctx.stroke()
                            }
                        }
                    }

                    Repeater {
                        model: AppState.boardItemsModel

                        delegate: Item {
                            x: 12 + ((itemX - AppState.boardOriginX) * viewport.fitScale * viewport.zoom)
                            y: 12 + ((itemY - AppState.boardOriginY) * viewport.fitScale * viewport.zoom)
                            width: itemWidth * viewport.fitScale * viewport.zoom
                            height: itemHeight * viewport.fitScale * viewport.zoom
                            rotation: itemRotation
                            transformOrigin: Item.Center
                            opacity: itemOpacity
                            z: itemZ

                            Rectangle {
                                anchors.fill: parent
                                visible: boardImage.status !== Image.Ready
                                radius: 12
                                color: "#f1d4c8"
                                border.width: 1
                                border.color: "#181818"
                            }

                            Image {
                                id: boardImage
                                anchors.fill: parent
                                source: itemSource
                                asynchronous: true
                                fillMode: Image.PreserveAspectFit
                                smooth: true
                                sourceClipRect: cropWidth > 0 && cropHeight > 0
                                    ? Qt.rect(cropX, cropY, cropWidth, cropHeight)
                                    : Qt.rect(0, 0, 0, 0)
                            }

                            Rectangle {
                                visible: itemLabel !== ""
                                rotation: -parent.rotation
                                transformOrigin: Item.TopLeft
                                x: labelLocalX
                                y: labelLocalY
                                width: labelText.implicitWidth + 14
                                height: labelText.implicitHeight + 8
                                radius: 4
                                color: Qt.rgba(0, 0, 0, 0.65)

                                property real itemCenterX: parent.width / 2
                                property real itemCenterY: parent.height / 2
                                property real radians: parent.rotation * Math.PI / 180.0
                                property real cosValue: Math.cos(radians)
                                property real sinValue: Math.sin(radians)
                                property var corners: [
                                    { sx: -itemCenterX * cosValue + itemCenterY * sinValue, sy: -itemCenterX * sinValue - itemCenterY * cosValue },
                                    { sx:  itemCenterX * cosValue + itemCenterY * sinValue, sy:  itemCenterX * sinValue - itemCenterY * cosValue },
                                    { sx:  itemCenterX * cosValue - itemCenterY * sinValue, sy:  itemCenterX * sinValue + itemCenterY * cosValue },
                                    { sx: -itemCenterX * cosValue - itemCenterY * sinValue, sy: -itemCenterX * sinValue + itemCenterY * cosValue }
                                ]
                                property real boundingMinX: Math.min(corners[0].sx, corners[1].sx, corners[2].sx, corners[3].sx)
                                property real boundingMinY: Math.min(corners[0].sy, corners[1].sy, corners[2].sy, corners[3].sy)
                                property real labelSceneX: itemCenterX + boundingMinX
                                property real labelSceneY: itemCenterY + boundingMinY - height - 4
                                property real offsetX: labelSceneX - itemCenterX
                                property real offsetY: labelSceneY - itemCenterY
                                property real labelLocalX: offsetX * cosValue + offsetY * sinValue + itemCenterX
                                property real labelLocalY: -offsetX * sinValue + offsetY * cosValue + itemCenterY

                                Text {
                                    id: labelText
                                    anchors.centerIn: parent
                                    text: itemLabel
                                    font.pixelSize: 14
                                    color: "white"
                                }
                            }
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        visible: AppState.boardItemsModel.count === 0 && !AppState.loadingBoard
                        text: "На этой доске пока нет изображений."
                        color: "#5b4d57"
                        font.pixelSize: 14
                    }
                }
            }

            PinchArea {
                anchors.fill: parent
                property real startZoom: 1.0

                onPinchStarted: startZoom = viewport.zoom
                onPinchUpdated: {
                    viewport.zoom = Math.max(viewport.minZoom, Math.min(viewport.maxZoom, startZoom * pinch.scale))
                    gridCanvas.requestPaint()
                }
            }
        }
    }
}
