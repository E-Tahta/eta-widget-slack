/*****************************************************************************
 *   Copyright (C) 2018 by Hikmet Baş                                        *
 *   <hikmet.bas@pardus.org.tr>                                              *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/

import QtQuick 1.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets

/**
 * This qml component works as declerative plasma widget and basically triggers
 * the Peach Kwin plugin via pushing key shortcuts to the system.
 */


Rectangle {
    id:root
    color:"transparent"
    smooth: true

    x: 75

    property string prevState: bell.state

    property int bellSize: 100
    property int bellRadius: 50

    property int stopRadius: 30
    property int listRadius: 10

    property int confirmButtonsSize: 50
    property int confirmButtonsRadius: 25

    property int listButtonSize: 30
    property int listButtonRadius: 15
    property int listMargins: 10

    property string textColor: "#eeeeee"
    property string message: "Dersi Bitir"

    property bool lecture: true


    Rectangle {

        id: bell
        width: root.bellSize
        height: root.bellSize
        radius: root.bellRadius
        smooth: true

        state: "STARTED"


        Text {
            id: txtBell
            color: root.textColor
            text: root.message
            font.family: "FreeSans"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11

            visible: bell.state == "LIST" ? false : true

            anchors.centerIn: parent
        }

        Text {
            id: txtLegend
            visible: bell.state == "LIST" ? true : false
            width: parent.width - root.listMargins * 2
            height: 20
            text: "Ders\nBaşlangıç\tSon\tTarih"
            font.bold: true
            color: root.textColor
            anchors {
                top: parent.top
                margins: root.listMargins
                horizontalCenter: parent.horizontalCenter
            }
        }
        
        Flickable {
            interactive: true
            visible: bell.state == "LIST" ? true : false
            width: parent.width - root.listMargins * 2
            height: parent.height - txtLegend.height - root.listMargins * 2
            clip: true
            anchors {
                top: txtLegend.bottom
                left: parent.left
                margins: root.listMargins

            }

            ListView {
                id:list
                width: parent.width
                height: parent.height - txtLegend.height
                anchors.top: txtLegend.bottom
                model: slack.getList()
                delegate: Text {
                    text:modelData
                    color: root.textColor
                    horizontalAlignment: Text.AlignHCenter
                }
                focus: true
            }
        }


        MouseArea {
            anchors.fill: parent
            visible: bell.state == "LIST" ? false : true

            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button == Qt.LeftButton) {
                    if (bell.state == "STARTED")  {
                        bell.state = "CONFIRMATION"
                        slack.playSound(bell.state)

                    } else if (bell.state == "STOPPED"){
                        bell.state = "STARTED"
                        slack.playSound(bell.state)
                    }

                }
                else {
                    if (bell.state != "CONFIRMATION") {
                        root.prevState = bell.state
                        bell.state = "LIST"
                        slack.playSound("LISTOPENED")
                    }
                }
            }
        }

        Rectangle {
            id: btnConfirm
            width: root.confirmButtonsSize
            height: root.confirmButtonsSize
            radius: root.confirmButtonsRadius
            color: "#8BC34A"
            smooth: true
            visible: bell.state == "CONFIRMATION" ? true : false

            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            Text {
                text: "Evet"
                font.bold: true
                color: root.textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent

            }

            MouseArea {
                id: maConfirm
                anchors.fill: parent
                onClicked: {
                    bell.state = "STOPPED"
                    slack.playSound(bell.state)
                }
            }
        }

        Rectangle {
            id: btnCancel
            width: root.confirmButtonsSize
            height: root.confirmButtonsSize
            radius: root.confirmButtonsRadius
            color: "#C2352A"
            smooth: true
            visible: bell.state == "CONFIRMATION" ? true : false

            anchors {
                left: parent.left
                bottom: parent.bottom
            }

            Text {
                text: "Hayır"
                font.bold: true
                color: root.textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
            }

            MouseArea {
                id: maCancel
                anchors.fill: parent
                onClicked: {
                    bell.state = "STARTED"
                }
            }
        }

        Rectangle {
            id: btnList
            width: root.listButtonSize
            height: root.listButtonSize
            radius: root.listButtonRadius
            color: "red"
            smooth: true
            visible: bell.state == "LIST" ? true : false

            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "X"
                color: root.textColor
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
            }


            MouseArea {
                id: maList
                anchors.fill: parent
                onClicked: {
                    bell.state = root.prevState
                    slack.playSound("LISTCLOSED")
                }
            }

        }


        states: [
            State {
                name: "STARTED"
                PropertyChanges { target: bell; color: "#FF6C00"}
                PropertyChanges { target: root; message: "Dersi Bitir"}
                PropertyChanges { target: root; lecture: true}



            },
            State {
                name: "STOPPED"
                PropertyChanges { target: bell; color: "#009688"}
                PropertyChanges { target: root; message: "Dersi Başlat"}
                PropertyChanges { target: root; lecture: false}


            },
            State {
                name: "CONFIRMATION"
                PropertyChanges { target: bell; color: "#FF6C00"}
                PropertyChanges { target: root; message: "Emin misiniz?"}

            },
            State {
                name: "LIST"
                PropertyChanges { target: bell; color: "#383838"}
            }
        ]

        transitions: [
            Transition {
                from: "STARTED"
                to: "CONFIRMATION"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize * 1.5; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize * 1.5; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.bellRadius * 1.5; duration: 200 }
                NumberAnimation { target: bell; property: "x"; to: root.bellSize * -0.25; duration: 200 }
                NumberAnimation { target: bell; property: "y"; to: root.bellSize * -0.25; duration: 200 }
            },

            Transition {
                from: "CONFIRMATION"
                to: "STOPPED"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.stopRadius; duration: 200 }
                NumberAnimation { target: bell; property: "x"; to: 0; duration: 200 }
                NumberAnimation { target: bell; property: "y"; to: 0; duration: 200 }
            },

            Transition {
                from: "CONFIRMATION"
                to: "STARTED"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.bellRadius; duration: 200 }
                NumberAnimation { target: bell; property: "x"; to: 0; duration: 200 }
                NumberAnimation { target: bell; property: "y"; to: 0; duration: 200 }
            },

            Transition {
                from: "STOPPED"
                to: "STARTED"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "radius"; to: root.bellRadius; duration: 200}

            },

            Transition {
                from: "STARTED"
                to: "LIST"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize * 2.6; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize * 4; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.listRadius; duration: 200}
                NumberAnimation { target: bell; property: "x"; to: root.bellSize * -0.8; duration: 200 }

            },
            Transition {
                from: "STOPPED"
                to: "LIST"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize * 2.6; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize * 4; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.listRadius; duration: 200}
                NumberAnimation { target: bell; property: "x"; to: root.bellSize * -0.8; duration: 200 }
            },

            Transition {
                from: "LIST"
                to: "STARTED"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.bellRadius; duration: 200 }
                NumberAnimation { target: bell; property: "x"; to: 0; duration: 200 }

            },

            Transition {
                from: "LIST"
                to: "STOPPED"
                ColorAnimation { target: bell; duration: 200}
                NumberAnimation { target: bell; property: "width"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "height"; to: root.bellSize; duration: 200 }
                NumberAnimation { target: bell; property: "radius"; to: root.stopRadius; duration: 200 }
                NumberAnimation { target: bell; property: "x"; to: 0; duration: 200 }

            }
        ]
    }

    Component.onCompleted: {
        slack.lecture = true
    }

    onLectureChanged: {
        slack.lecture = root.lecture
    }
}
