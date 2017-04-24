import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.androidutils.demo 1.0

Pane {

	PrefModel {
		id: pModel
	}

	GridLayout {
		anchors.fill: parent
		columns: 3

		TextField {
			id: keyField
			placeholderText: "key"
		}

		TextField {
			id: valueField
			placeholderText: "value"
			Layout.fillWidth: true
		}

		ToolButton {
			text: "+"
			onClicked: pModel.save(keyField.text, valueField.text)
		}

		ListView {
			model: pModel.model

			Layout.columnSpan: 3
			Layout.fillWidth: true
			Layout.fillHeight: true

			delegate: ItemDelegate {
				text: modelData
				width: parent.width

				onPressAndHold: pModel.remove(index)
			}
		}
	}
}
