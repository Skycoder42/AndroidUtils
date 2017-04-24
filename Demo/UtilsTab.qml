import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.androidutils 1.1

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
