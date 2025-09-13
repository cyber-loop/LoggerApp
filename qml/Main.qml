import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5

ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: "Logger App"
    // Fixed column widths for the List data
    property var colWidths: [60, 150, 200, 150, 150]

    Frame {
        anchors.fill: parent
        padding: 12

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            // Row for filters
            RowLayout {
                spacing: 8
                Layout.fillWidth: true

                ComboBox {
                    id: typeBox
                    model: ListModel {}
                    textRole: "display"
                    Layout.fillWidth: true
                    Connections {
                        target: loggerModel
                        function onDataChanged() {
                            // Save the current selection
                            var currentSelection = typeBox.currentText
                            typeBox.model.clear()
                            var set = {}
                            for (var i=0; i<loggerModel.rowCount(); ++i) {
                                var type = loggerModel.get(i).type
                                if (type && !set[type]) {
                                    set[type] = true
                                    typeBox.model.append({"display": type})
                                }
                            }
                            // Insert empty option at the top
                            typeBox.model.insert(0, {"display": ""})

                            // Restore previous selection if still available
                            for (var j = 0; j < typeBox.model.count; ++j) {
                                if (typeBox.model.get(j).display === currentSelection) {
                                    typeBox.currentIndex = j
                                    break
                                }
                            }
                        }
                    }
                    onCurrentTextChanged: loggerProxy.setTypeFilter(currentText)
                }

                ComboBox {
                    id: locationBox
                    model: ListModel {}
                    textRole: "display"
                    Layout.fillWidth: true
                    Connections {
                        target: loggerModel
                        function onDataChanged() {
                            // Save the current selection
                            var currentSelection = locationBox.currentText
                            locationBox.model.clear()
                            var set = {}
                            for (var i=0; i<loggerModel.rowCount(); ++i) {
                                var loc = loggerModel.get(i).location
                                if (loc && !set[loc]) {
                                    set[loc] = true
                                    locationBox.model.append({"display": loc})
                                }
                            }
                            // Insert empty option at the top
                            locationBox.model.insert(0, {"display": ""})

                            // Restore previous selection if still available
                            for (var j = 0; j < locationBox.model.count; ++j) {
                                if (locationBox.model.get(j).display === currentSelection) {
                                    locationBox.currentIndex = j
                                    break
                                }
                            }
                        }
                    }
                    onCurrentTextChanged: loggerProxy.setLocationFilter(currentText)
                }

                Button {
                    text: loggerModel ? (loggerModel.loading ? "Loading..." : "Refresh") : "Refresh"
                    enabled: loggerModel ? !loggerModel.loading : false
                    onClicked: if(loggerModel) loggerModel.refresh()
                }

                Text {
                    text: loggerModel ? "State: " + loggerModel.connectionState : "State: -"
                    color: (loggerModel && loggerModel.connectionState === "Connected") ? "green" :
                           (loggerModel && loggerModel.connectionState === "Connecting...") ? "orange" : "red"
                }
            }
            // Header row
            Row {
                spacing: 0
                Repeater {
                    model: ["ID", "Type", "Location", "Temperature", "Humidity"]
                    delegate: Rectangle {
                        width: colWidths[index]
                        height: 30
                        color: "#e0e0e0"
                        border.color: "#b0b0b0"
                        Text {
                            anchors.centerIn: parent
                            font.bold: true
                            text: modelData
                        }
                    }
                }
            }
            // Data rows
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: loggerProxy
                delegate: Row {
                    spacing: 0
                    height: 28
                    Repeater {
                        model: 5
                        delegate: Rectangle {
                            width: colWidths[index]
                            height: 28
                            color: "white"
                            border.color: "#d0d0d0"
                            Text {
                                anchors.centerIn: parent
                                text: {
                                    switch (index) {
                                    case 0: return id
                                    case 1: return type
                                    case 2: return location
                                    case 3: return (typeof temperature === "number") ? temperature.toFixed(1) + " °C" : ""
                                    case 4: return (typeof humidity === "number") ? humidity.toFixed(1) + " %" : ""
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
