import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1

Item {
    id: container

    ColumnLayout {
        anchors.fill: parent
        spacing: 6

        Slider {
            id: param1
            from: 0
            to: 100
            value: 50
        }

        Button {
            text: "Test"
            onClicked: deskChan.sendMessage("dc:say", {"text": "Test"})
        }
    }
}
