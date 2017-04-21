import Qt.labs.platform 1.0
import de.skycoder42.androidutils 1.0

FileDialog {
	id: fileDialog

	property bool linkFileMode: true

	property alias contentUrl: fileDialog.file
	property int type: FileChooser.OpenDocument
	property string mimeType: "*/*"
	property int chooserFlags: FileChooser.OpenableFlag

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
