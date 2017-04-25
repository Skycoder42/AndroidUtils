import QtQuick 2.8
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import de.skycoder42.androidutils 1.1

FileChooser {
	id: fileDialog

	property var labsDialog: null
	property alias fileChooser: fileDialog

	property bool saveFile: false
	property int dialogFlags: 0
	property string defaultSuffix
	property string folder
	property alias file: fileDialog.contentUrl
	property var nameFilters

	type: saveFile ? FileChooser.CreateDocument : FileChooser.OpenDocument
	contentUrl: folder + "file" + defaultSuffix
}
