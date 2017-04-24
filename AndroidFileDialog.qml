import QtQuick 2.8
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import de.skycoder42.androidutils 1.1

FileChooser {
	id: fileDialog

	property bool linkFileMode: true

	default property var data
	property int flags: Qt.Dialog
	property int modality: Qt.NonModal
	property Window parentWindow: null
	property int result: Dialog.Rejected
	property bool visible: false
	property string acceptLabel
	property string rejectLabel
	property alias currentFile: fileDialog.contentUrl
	property var currentFiles: [currentFile]
	property string defaultSuffix
	property alias file: fileDialog.contentUrl
	property int fileMode: FileDialog.OpenFile
	property var files: [file]
	property alias folder: fileDialog.contentUrl
	property var nameFilters
	property int options

	onAccepted: result = Dialog.Accepted
	onRejected: result = Dialog.Rejected

	function accept() {}
	function reject() {}
	function close() {}
	function done(result) {
		fileDialog.result = result;
	}

	onTypeChanged: {
		if(!linkFileMode)
			return;

		switch(type) {
		case FileChooser.GetContent:
		case FileChooser.OpenDocument:
			fileMode = FileDialog.OpenFile;
			break;
		case FileChooser.CreateDocument:
			fileMode = FileDialog.SaveFile
			break;
		}
	}

	onFileModeChanged: {
		if(!linkFileMode)
			return;

		switch(fileMode) {
		case FileDialog.OpenFile:
		case FileDialog.OpenFiles:
			fileMode = FileChooser.OpenDocument;
			break;
		case FileDialog.SaveFile:
			fileMode = FileChooser.CreateDocument;
			break;
		}
	}
}
