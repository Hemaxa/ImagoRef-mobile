import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "pages"

ApplicationWindow {
    id: root

    visible: true
    width: 393
    height: 780
    minimumWidth: 360
    minimumHeight: 720
    title: "ImagoRef Mobile"
    color: "transparent"

    background: Rectangle {
        color: ThemeManager.colors.welcomeBgColor
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: homePageComponent
    }

    Component {
        id: homePageComponent

        HomePage {
            onBoardRequested: function(boardId, boardTitle) {
                AppState.openBoard(boardId, boardTitle)
                stackView.push(boardPageComponent)
            }
        }
    }

    Component {
        id: boardPageComponent

        BoardPage {
            onBackRequested: {
                AppState.closeBoard()
                stackView.pop()
            }
        }
    }
}
