import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0

Rectangle {
    width: 500
    height : 500
    color : "white"
    id : root
    property int largeurBouton : 140 // Context.largeurBouton

    ComboBox {
        x:292
        y:28
        id : idComboFiltres
        visible : false
        currentIndex: 0
        model: Filtres
        width: 70
        onCurrentIndexChanged:{
            Context.sortList(navigateur.folder, idComboFiltres.currentText)
        }
    }

    TableView {
        id: tableView
        width: 276; height: 295
        x: 209
        y: 90
        selectionMode: SelectionMode.MultiSelection
        TableViewColumn
        {
            role : "Fichiers"
            title: "               Fichiers"
            width: 200
        }
        model: lvModel
    }

    Rectangle {
        id: rectangle
        x: 210
        y: 431
        width: 275
        height: 19
        color: "light grey"

        TextInput {
            id: textInput
            x: 0
            y: 0
            width: 251
            height: 20
            text:"C:/testqt/"
            font.pixelSize: 12
            selectByMouse: true
            onAccepted: {
                var ligne = ''
                tableView.selection.forEach(function(rowIndex) {
                    ligne += tableView.model[rowIndex] + ";"
                } )
                Context.write(ligne, textInput.text)
            }
        }
    }

    Text {
        id: text5
        x: 311
        y: 8
        width: 32
        height: 14
        text: qsTr("Filtres")
        font.pixelSize: 12
    }

    Text {
        id: text6
        x: 209
        y: 393
        width: 219
        height: 32
        text: qsTr("Entrez le chemin et le nom ci-dessous                    Ex: chemin/nomfichier.extension")
        wrapMode: Text.WordWrap
        font.pixelSize: 12
    }

    Image {
        id: image
        x: 374
        y: 0
        width: 126
        height: 75
        source: "../../../../Desktop/logo afpa.png"
    }

    Column {
        id: column
        x: 9
        y: 72
        width: 200
        height: 400
        spacing: 20

        Text{
            height : 59
            width : 140
            color : "red"
            text: qsTr("Bienvenue ! Pour utiliser le merger de fichiers veuillez suivre les instructions suivantes : ")
            wrapMode: Text.WordWrap
            verticalAlignment : TextEdit.AlignVCenter
        }

        Text {
            id: text1
            width: 136
            height: 44
            text: qsTr("1) Choisissez votre dossier avec ''Chose a Folder''")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }

        Button{
            width : root.largeurBouton
            text : "Chose a Folder"
            onClicked : {
                navigateur.visible=true
            }
        }

        Text {
            id: text2
            width: 132
            height: 32
            text: qsTr("2) Sélectionner votre filtre dans la combo")
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }

        Text {
            id: text3
            width: 117
            height: 46
            text: qsTr("3) Entrez le chemin et le nom du fichier de destination dans le champs prévu à cet effet")
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }

        Text {
            id: text4
            width: 106
            height: 36
            text: qsTr("4) Appuyez sur ''Entrée'' et voilà !!")
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }





        Button {
            width : Context.largeurBouton
            text : "QUITTER"
            onClicked : Context.quit()


        }

    }

    Text {
        id: text7
        x: 29
        y: 24
        width: 161
        height: 28
        text: qsTr("FILE MERGER")
        font.bold: true
        font.family: "Arial"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 18
    }

    FileDialog {
        id: navigateur
        visible: false
        selectFolder: true
        onAccepted: { Context.sendUrlToCpp(navigateur.folder)
            idComboFiltres.visible=true
        }
    }


}
