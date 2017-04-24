import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.androidutils 1.0
import de.skycoder42.androidutils.demo 1.0

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

			currentIndex: 1
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
