import Qt.labs.platform 1.0 as Labs
import de.skycoder42.androidutils 1.1

Labs.FileDialog {
	id: fileDialog

	property alias labsDialog: fileDialog
	property var fileChooser: null

	property bool saveFile: false
	property alias dialogFlags: fileDialog.options
	property int chooserFlags: FileChooser.OpenableFlag
	property string mimeType: "*/*"

	fileMode: saveFile ? Labs.FileDialog.SaveFile : Labs.FileDialog.OpenFile
	modality: Qt.WindowModal
}
