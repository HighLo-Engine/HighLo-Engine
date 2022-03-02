// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-02) initial release
//

#pragma once

#include <HighLo.h>
using namespace highlo;

static void InitGermanTranslations()
{
	Translation *german = HLApplication::Get().GetTranslations().GetTranslation("de-DE");
	if (german)
	{
		german->AddText("file-menu", "Datei");
		german->AddText("new-scene-file-menu", "Neue Szene");
		german->AddText("open-scene-file-menu", u8"Szene öffnen..."); // TODO: make another AddText function that accepts UTF-8 Strings
		german->AddText("save-scene-file-menu", "Szene speichern");
		german->AddText("save-scene-as-file-menu", "Szene speichern als...");
		german->AddText("import-file-menu", "Importieren");
		german->AddText("import-obj-file-menu", ".obj importieren");
		german->AddText("import-fbx-file-menu", ".fbx importieren");
		german->AddText("import-stl-file-menu", ".stl importieren");
		german->AddText("import-3ds-file-menu", ".3ds importieren");
		german->AddText("import-c4d-file-menu", ".c4d importieren");
		german->AddText("import-mb-file-menu", ".mb importieren");
		german->AddText("export-file-menu", "Exportieren");
		german->AddText("export-obj-file-menu", ".obj exportieren");
		german->AddText("export-fbx-file-menu", ".fbx exportieren");
		german->AddText("export-stl-file-menu", ".stl exportieren");
		german->AddText("export-mp4-file-menu", ".mp4 exportieren");
		german->AddText("export-avi-file-menu", ".avi exportieren");
		german->AddText("export-mov-file-menu", ".mov exportieren");
		german->AddText("settings-file-menu", "Einstellungen");
		german->AddText("exit-file-menu", "Beenden");

		german->AddText("edit-menu", "Bearbeiten");
		german->AddText("edit-undo-menu", u8"Rückgängig");
		german->AddText("edit-redo-menu", "Wiederholen");

		german->AddText("game-objects-menu", "Spielobjekte");
		german->AddText("game-objects-folder-menu", "Ordner erstellen");
		german->AddText("game-objects-camera-menu", "Kamera erstellen");
		german->AddText("game-objects-3d-objects-menu", "3D-Objekte erstellen");
		german->AddText("game-objects-3d-objects-null-menu", "Null Objekt erstellen");
		german->AddText("game-objects-3d-objects-cube-menu", u8"Würfel erstellen");
		german->AddText("game-objects-3d-objects-sphere-menu", "Kugel erstellen");
		german->AddText("game-objects-3d-objects-capsule-menu", "Kapsel erstellen");
		german->AddText("game-objects-3d-objects-cylinder-menu", "Zylinder erstellen");

		german->AddText("renderer-settings-menu", "Renderer Einstellungen");

		german->AddText("window-menu", "Fenster");
		german->AddText("window-editor-console-menu", "Editor Konsole");
		german->AddText("window-asset-manager-menu", "Asset Manager");
		german->AddText("window-animation-timeline-menu", "Animationen-Zeitleiste");

		german->AddText("help-menu", "Hilfe");
		german->AddText("help-about-menu", u8"Über HighLo");
		german->AddText("help-docs-menu", "Dokumentation");
	}
}

static void InitEnglishTranslations()
{
	Translation *english = HLApplication::Get().GetTranslations().GetTranslation("en-EN");
	if (english)
	{
		english->AddText("file-menu", "File");
		english->AddText("new-scene-file-menu", "New Scene");
		english->AddText("open-scene-file-menu", "Open Scene...");
		english->AddText("save-scene-file-menu", "Save Scene");
		english->AddText("save-scene-as-file-menu", "Save Scene as...");
		english->AddText("import-file-menu", "Import");
		english->AddText("import-obj-file-menu", "Import .obj");
		english->AddText("import-fbx-file-menu", "Import .fbx");
		english->AddText("import-stl-file-menu", "Import .stl");
		english->AddText("import-3ds-file-menu", "Import .3ds");
		english->AddText("import-c4d-file-menu", "Import .c4d");
		english->AddText("import-mb-file-menu", "Import .mb");
		english->AddText("export-file-menu", "Export");
		english->AddText("export-obj-file-menu", "Export .obj");
		english->AddText("export-fbx-file-menu", "Export .fbx");
		english->AddText("export-stl-file-menu", "Export .stl");
		english->AddText("export-mp4-file-menu", "Export .mp4");
		english->AddText("export-avi-file-menu", "Export .avi");
		english->AddText("export-mov-file-menu", "Export .mov");
		english->AddText("settings-file-menu", "Settings");
		english->AddText("exit-file-menu", "Quit");

		english->AddText("edit-menu", "Edit");
		english->AddText("edit-undo-menu", "Undo");
		english->AddText("edit-redo-menu", "Redo");

		english->AddText("game-objects-menu", "Game objects");
		english->AddText("game-objects-folder-menu", "Create Folder");
		english->AddText("game-objects-camera-menu", "Create Camera");
		english->AddText("game-objects-3d-objects-menu", "Create 3D Objects");
		english->AddText("game-objects-3d-objects-null-menu", "Create Null object");
		english->AddText("game-objects-3d-objects-cube-menu", "Create Cube");
		english->AddText("game-objects-3d-objects-sphere-menu", "Create Sphere");
		english->AddText("game-objects-3d-objects-capsule-menu", "Create Capsule");
		english->AddText("game-objects-3d-objects-cylinder-menu", "Create Cylinder");

		english->AddText("renderer-settings-menu", "Renderer Settings");

		english->AddText("window-menu", "Window");
		english->AddText("window-editor-console-menu", "Editor Console");
		english->AddText("window-asset-manager-menu", "Asset Manager");
		english->AddText("window-animation-timeline-menu", "Animation Timeline");

		english->AddText("help-menu", "Help");
		english->AddText("help-about-menu", "About HighLo");
		english->AddText("help-docs-menu", "Documentation");
	}
}

static void InitEditorTranslations()
{
	InitGermanTranslations();
	InitEnglishTranslations();
}

