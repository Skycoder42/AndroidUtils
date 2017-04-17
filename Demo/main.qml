import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import de.skycoder42.androidutils 1.0

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	Page {
		anchors.fill: parent
		Pane {
			anchors.fill: parent
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
	}
}
