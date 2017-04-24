import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.androidutils 1.1
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

		TabButton {
			id: btn2
			text: "Preferences"
		}
	}

	Page {
		anchors.fill: parent

		SwipeView {
			id: swipeView
			anchors.fill: parent
			currentIndex: tabBar.currentIndex

			UtilsTab {}
			ContentTab {}
			PreferenceTab {}
		}
	}
}
