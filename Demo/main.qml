import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import de.skycoder42.androidutils 1.0
import Qt.labs.platform 1.0 as Labs//fix because of file tree

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	footer: TabBar {
		id: tabBar
		currentIndex: swipeView.currentIndex

		TabButton {
			id: btn0
			text: "Utils"
		}

		TabButton {
			id: btn1
			text: "File Chooser"
		}
	}

	Page {
		anchors.fill: parent

		SwipeView {
			id: swipeView
			anchors.fill: parent
			currentIndex: tabBar.currentIndex

			Pane {
				ColumnLayout {
					anchors.centerIn: parent

					TextField {
						id: color
						placeholderText: "Enter a hex color value"
						inputMask: "\\#HHHHHH"
						text: "#ABCDEF"
					}
					Button {
						id: changeButton
						text: "Change color"
						onClicked: AndroidUtils.setStatusBarColor(color.text)
					}

					CheckBox {
						id: longBox
						text: "Long toast"
					}
					Button {
						id: toastButton
						text: "Show Toast"
						onClicked: AndroidUtils.showToast("This is a toast", longBox.checked)
					}

					Button {
						id: longButton
						text: "Press me long!"
						onPressAndHold: AndroidUtils.hapticFeedback(AndroidUtils.LongPress)
					}
				}
			}

			Pane {
				FileDialog {
					id: chooser

					type: modeBox.model.get(modeBox.currentIndex).value

					onAccepted: AndroidUtils.showToast("File choosen: " + chooser.contentUrl, true)
					onRejected: AndroidUtils.showToast("file choosing aborted!")
				}

				Opener {
					id: opener

					onDataLoaded: editField.text = data
				}

				GridLayout {
					anchors.fill: parent
					columns: 3

					TextField {
						id: contentField
						text: chooser.contentUrl

						Layout.fillWidth: true
						Layout.columnSpan: 3

						onEditingFinished: chooser.contentUrl = contentField.text
					}

					ComboBox {
						id: modeBox
						textRole: "key"

						Layout.fillWidth: true
						Layout.columnSpan: 3

						model: ListModel {
							ListElement { key: "GetContent"; value: FileChooser.GetContent }
							ListElement { key: "OpenDocument"; value: FileChooser.OpenDocument }
							ListElement { key: "CreateDocument"; value: FileChooser.CreateDocument }
						}

						currentIndex: 0
					}

					Button {
						id: chooserButton
						Layout.fillWidth: true
						text: "Open file chooser"
						onClicked: chooser.open()
					}

					Button {
						id: editButton
						Layout.fillWidth: true
						text: "Display/Edit File"

						onClicked: opener.openFile(chooser.contentUrl)
					}

					Button {
						id: saveButton
						Layout.fillWidth: true
						text: "Save File"

						onClicked: opener.saveFile(chooser.contentUrl, editField.text)
					}

					TextArea {
						id: editField
						Layout.fillHeight: true
						Layout.fillWidth: true
						Layout.columnSpan: 3
					}
				}
			}
		}
	}
}
