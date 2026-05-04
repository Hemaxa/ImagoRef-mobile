import QtQuick

Item {
    Image {
        source: ThemeManager.icons.triangles_1 || ""
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: -8
        anchors.topMargin: 14
    }

    Image {
        source: ThemeManager.icons.dots || ""
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 18
        anchors.topMargin: -32
        scale: 0.9
    }

    Image {
        source: ThemeManager.icons.star_1 || ""
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 64
        anchors.topMargin: 148
        scale: 0.85
    }

    Image {
        source: ThemeManager.icons.path_2 || ""
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 28
        anchors.topMargin: 248
        scale: 0.75
    }

    Image {
        source: ThemeManager.icons.rectangles_1 || ""
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: -26
        anchors.topMargin: 284
        scale: 0.85
    }

    Image {
        source: ThemeManager.icons.circles_3 || ""
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 254
        anchors.topMargin: 104
        scale: 0.75
    }

    Image {
        source: ThemeManager.icons.lines_star || ""
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 24
        anchors.bottomMargin: 136
        scale: 0.7
        rotation: 64
    }

    Image {
        source: ThemeManager.icons.path_1 || ""
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -18
        anchors.bottomMargin: 164
        scale: 0.8
    }

    Image {
        source: ThemeManager.icons.form_4 || ""
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -8
        anchors.bottomMargin: 38
        scale: 0.78
    }
}
