import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.androidutils 1.1
import de.skycoder42.androidutils.demo 1.0

Pane {
	FileDialog {
		id: fileDialog

		saveFile: openBox.checked

		onAccepted: AndroidUtils.showToast("File choosen: " + fileDialog.file, true)
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
			text: fileDialog.file

			Layout.fillWidth: true
			Layout.columnSpan: 2

			onEditingFinished: chooser.contentUrl = contentField.text
		}

		CheckBox {
			id: openBox
			text: "Get save file"
		}

		Button {
			id: chooserButton
			Layout.fillWidth: true
			text: "Open file chooser"
			onClicked: fileDialog.open()
		}

		Button {
			id: editButton
			Layout.fillWidth: true
			text: "Display/Edit File"

			onClicked: opener.openFile(fileDialog.file)
		}

		Button {
			id: saveButton
			Layout.fillWidth: true
			text: "Save File"

			onClicked: opener.saveFile(fileDialog.file, editField.text)
		}

		TextArea {
			id: editField
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.columnSpan: 3
		}
	}
}
