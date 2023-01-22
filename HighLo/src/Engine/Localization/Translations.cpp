#include "HighLoPch.h"
#include "Translations.h"

namespace highlo
{
	std::vector<Translation> Translations::s_Translations;

	void Translations::Init()
	{
		// German translations
		Translation german("German", "de-DE");

		// Animation timeline
		german.AddText("animation-timeline-play-btn-tooltip", "Play\n\nAnimation abspielen");
		german.AddText("animation-timeline-pause-btn-tooltip", "Pause\n\nAnimation pausieren");
		german.AddText("animation-timeline-prev-frame-btn-tooltip", "Vorheriges Bild\n\nZum vorherigen Bild springen");
		german.AddText("animation-timeline-next-frame-btn-tooltip", u8"Nächstes Bild\n\nZum nächsten Bild springen");
		german.AddText("animation-timeline-begin-btn-tooltip", "Zum Anfang\n\nZum Anfang der Animation springen");
		german.AddText("animation-timeline-end-btn-tooltip", "Zum Ende\n\nZum Ende der Animation springen");
		german.AddText("animation-timeline-keyframe-btn-tooltip", "Keyframe erstellen\n\nKeyframe von der Position, Rotation und Skalierung erstellen");
		german.AddText("animation-timeline-autokeying-btn-tooltip", "Autokeying\n\nVollautomatisierte Keyframe Aufnahme");

		// Asset Browser
		german.AddText("asset-browser-content-headline", "Inhalt");
		german.AddText("asset-browser-prev-dir-btn-tooltip", "Vorheriges Verzeichnis\n\nSpringt zum vorherigen Verzeichnis, falls ein vorheriges Verzeichnis exisitert");
		german.AddText("asset-browser-next-dir-btn-tooltip", u8"Nächstes Verzeichnis\n\nSpring zum nächsten Verzeichnis, falls vorher zurück gegangen wurde");
		german.AddText("asset-browser-refresh-dir-btn-tooltip", "Aktualisieren\n\nSynchronisiert den aktuell angezeigten Ordner im AssetBrowser mit dem entsprechenden Ordner auf der Festplatte.");
		german.AddText("asset-browser-right-click-menu-new", "Neu");
		german.AddText("asset-browser-right-click-menu-new-folder", "Ordner");
		german.AddText("asset-browser-right-click-menu-new-scene", "Szene");
		german.AddText("asset-browser-right-click-menu-import", "Importieren");
		german.AddText("asset-browser-right-click-menu-refresh", "Aktualisieren");
		german.AddText("asset-browser-right-click-menu-copy", "Kopieren");
		german.AddText("asset-browser-right-click-menu-paste", u8"Einfügen");
		german.AddText("asset-browser-right-click-menu-duplicate", "Duplizieren");
		german.AddText("asset-browser-right-click-menu-explorer", "In Explorer anzeigen");

		german.AddText("asset-browser-item-reload", "Asset neu laden");
		german.AddText("asset-browser-item-rename", "Asset umbenennen");
		german.AddText("asset-browser-item-copy", "Asset kopieren");
		german.AddText("asset-browser-item-delete", u8"Asset löschen");
		german.AddText("asset-browser-item-explorer", u8"Asset in Explorer �ffnen");

		// Scene Hierarchy Panel
		german.AddText("scene-hierarchy-window-title", "Szenen Hierarchie");
		german.AddText("scene-hierarchy-right-click-menu-new", "Erstellen...");
		german.AddText("scene-hierarchy-right-click-menu-new-null-object", "Null Objekt");
		german.AddText("scene-hierarchy-right-click-menu-new-camera", "Kamera");
		german.AddText("scene-hierarchy-right-click-menu-new-cube", u8"Würfel");
		german.AddText("scene-hierarchy-right-click-menu-new-sphere", "Kugel");
		german.AddText("scene-hierarchy-right-click-menu-new-capsule", "Kapsel");
		german.AddText("scene-hierarchy-right-click-menu-new-cylinder", "Zylinder");
		german.AddText("scene-hierarchy-right-click-menu-new-directional-light", "Spot Lichtquelle");
		german.AddText("scene-hierarchy-right-click-menu-new-point-light", "Punkt Lichtquelle");
		german.AddText("scene-hierarchy-right-click-menu-new-sky-light", "Himmel");
		german.AddText("scene-hierarchy-right-click-menu-new-c#-script", "C# Skript");
		german.AddText("scene-hierarchy-right-click-menu-new-lua-script", "Lua Skript");
		german.AddText("scene-hierarchy-right-click-menu-duplicate-entity", "Entity duplizieren");
		german.AddText("scene-hierarchy-right-click-menu-show-entity", "Entity anzeigen");
		german.AddText("scene-hierarchy-right-click-menu-hide-entity", "Entity verstecken");
		german.AddText("scene-hierarchy-right-click-menu-remove-entity", "Entity entfernen");
		german.AddText("scene-hierarchy-right-click-menu-update-prefab", "Prefab aktualisieren");

		// Object Properties Panel
		german.AddText("object-properties-window-title", "Objekt Eigenschaften");
		german.AddText("object-properties-reset-component", u8"Komponent zurücksetzen");
		german.AddText("object-properties-remove-component", "Komponent entfernen");
		german.AddText("object-properties-transform-component", "Transformation");
		german.AddText("object-properties-transform-component-scale", "Skalierung");
		german.AddText("object-properties-camera-component", "Kamera");
		german.AddText("object-properties-static-model-component", "Statisches Modell");
		german.AddText("object-properties-dynamic-model-component", "Dynamisches Modell");
		german.AddText("object-properties-directional-light-component", "Spot Lichtquelle");
		german.AddText("object-properties-point-light-component", "Punkt Lichtquelle");
		german.AddText("object-properties-sky-light-component", "Himmel");
		german.AddText("object-properties-script-c#-component", "C# Skript");
		german.AddText("object-properties-script-lua-component", "Lua Skript");

		german.AddText("object-properties-camera-component-perspective", "Perspektivisch");
		german.AddText("object-properties-camera-component-orthographic", "Orthografisch");
		german.AddText("object-properties-camera-component-primary", u8"Prim�r Kamera?");
		german.AddText("object-properties-camera-component-projection", "Projektion");
		german.AddText("object-properties-camera-component-orthographic-size", u8"Orthgrafische Größe");
		german.AddText("object-properties-camera-component-vertical-fov", "Vertikale FOV");
		german.AddText("object-properties-camera-component-near-far-planes", "Nah- und Fern-ebenen");

		german.AddText("object-properties-light-component-radiance", "Farbe");
		german.AddText("object-properties-light-component-intensity", u8"Intensität");
		german.AddText("object-properties-light-component-radius", "Radius");
		german.AddText("object-properties-light-component-falloff", u8"Übergang");
		german.AddText("object-properties-light-component-cast-shadows", "Schatten werfen");
		german.AddText("object-properties-light-component-soft-shadows", "Weiche Schatten");
		german.AddText("object-properties-light-component-source-size", u8"Quellgröße");
		german.AddText("object-properties-light-component-dynamic-sky", "Dynamischer Himmel");

		german.AddText("object-properties-materials", "Materialien");
		german.AddText("object-properties-unnamed-material", "Unbenanntes Material");


		// ImGui Widgets
		german.AddText("imgui-widgets-search-widget-placeholder", "Suche...");

		s_Translations.push_back(german);

		// English translations
		Translation english("English", "en-US");

		// Animation timeline
		english.AddText("animation-timeline-play-btn-tooltip", "Play\n\nPlay the animation");
		english.AddText("animation-timeline-pause-btn-tooltip", "Pause\n\nPause the animation");
		english.AddText("animation-timeline-prev-frame-btn-tooltip", "Go to the previous frame\n\nJump to the previous frame");
		english.AddText("animation-timeline-next-frame-btn-tooltip", "Go to the next frame\n\nJump to the next frame");
		english.AddText("animation-timeline-begin-btn-tooltip", "Go to the beginning\n\nJump to the beginning of the animation");
		english.AddText("animation-timeline-end-btn-tooltip", "Go to the end\n\nJump to the end of the animation");
		english.AddText("animation-timeline-keyframe-btn-tooltip", "Create keyframe\n\nCreate a keyframe of the position, scale and rotation of the current selected object.");
		english.AddText("animation-timeline-autokeying-btn-tooltip", "Autokeying\n\nFully-automated keyframe recording");

		// Asset Browser
		english.AddText("asset-browser-content-headline", "Content");
		english.AddText("asset-browser-prev-dir-btn-tooltip", "Previous Directory\n\nJumps the the previous folder, if one exists");
		english.AddText("asset-browser-next-dir-btn-tooltip", "Next Directory\n\nJumps to the next directoy, if you previously jumped back");
		english.AddText("asset-browser-refresh-dir-btn-tooltip", "Refresh\n\nSynchronizes the shown folder in the asset browser with the matching folder on the hard drive");
		english.AddText("asset-browser-right-click-menu-new", "New");
		english.AddText("asset-browser-right-click-menu-new-folder", "Folder");
		english.AddText("asset-browser-right-click-menu-new-scene", "Scene");
		english.AddText("asset-browser-right-click-menu-import", "Import");
		english.AddText("asset-browser-right-click-menu-refresh", "Refresh");
		english.AddText("asset-browser-right-click-menu-copy", "Copy");
		english.AddText("asset-browser-right-click-menu-paste", "Paste");
		english.AddText("asset-browser-right-click-menu-duplicate", "Duplicate");
		english.AddText("asset-browser-right-click-menu-explorer", "Show in explorer");

		english.AddText("asset-browser-item-reload", "reload asset");
		english.AddText("asset-browser-item-rename", "rename asset");
		english.AddText("asset-browser-item-copy", "copy asset");
		english.AddText("asset-browser-item-delete", "delete asset");
		english.AddText("asset-browser-item-explorer", "open asset in explorer");

		// Scene Hierarchy Panel
		english.AddText("scene-hierarchy-window-title", "Scene Hierarchy");
		english.AddText("scene-hierarchy-right-click-menu-new", "Create...");
		english.AddText("scene-hierarchy-right-click-menu-new-null-object", "Null object");
		english.AddText("scene-hierarchy-right-click-menu-new-camera", "Camera");
		english.AddText("scene-hierarchy-right-click-menu-new-cube", "Cube");
		english.AddText("scene-hierarchy-right-click-menu-new-sphere", "Sphere");
		english.AddText("scene-hierarchy-right-click-menu-new-capsule", "Capsule");
		english.AddText("scene-hierarchy-right-click-menu-new-cylinder", "Cylinder");
		english.AddText("scene-hierarchy-right-click-menu-new-directional-light", "Directional Light");
		english.AddText("scene-hierarchy-right-click-menu-new-point-light", "Point Light");
		english.AddText("scene-hierarchy-right-click-menu-new-sky-light", "Sky Light");
		english.AddText("scene-hierarchy-right-click-menu-new-c#-script", "C# script");
		english.AddText("scene-hierarchy-right-click-menu-new-lua-script", "Lua script");
		english.AddText("scene-hierarchy-right-click-menu-duplicate-entity", "Duplicate entity");
		english.AddText("scene-hierarchy-right-click-menu-show-entity", "Show entity");
		english.AddText("scene-hierarchy-right-click-menu-hide-entity", "Hide entity");
		english.AddText("scene-hierarchy-right-click-menu-remove-entity", "Remove entity");
		english.AddText("scene-hierarchy-right-click-menu-update-prefab", "Update Prefab");

		// Object Properties Panel
		english.AddText("object-properties-window-title", "Object Properties");
		english.AddText("object-properties-reset-component", "Reset component");
		english.AddText("object-properties-remove-component", "Remove component");
		english.AddText("object-properties-transform-component", "Transform");
		english.AddText("object-properties-transform-component-scale", "Scale");
		english.AddText("object-properties-camera-component", "Camera");
		english.AddText("object-properties-static-model-component", "Static Model");
		english.AddText("object-properties-dynamic-model-component", "Dynamic Model");
		english.AddText("object-properties-directional-light-component", "Directional Light");
		english.AddText("object-properties-point-light-component", "Point Light");
		english.AddText("object-properties-sky-light-component", "Sky Light");
		english.AddText("object-properties-script-c#-component", "C# script");
		english.AddText("object-properties-script-lua-component", "Lua script");

		english.AddText("object-properties-camera-component-perspective", "Perspective");
		english.AddText("object-properties-camera-component-orthographic", "Orthographic");
		english.AddText("object-properties-camera-component-primary", "Primary Camera?");
		english.AddText("object-properties-camera-component-projection", "Projection");
		english.AddText("object-properties-camera-component-orthographic-size", "Orthographic Size");
		english.AddText("object-properties-camera-component-vertical-fov", "Vertical FOV");
		english.AddText("object-properties-camera-component-near-far-planes", "Near- and Farplanes");

		english.AddText("object-properties-light-component-radiance", "Radiance");
		english.AddText("object-properties-light-component-intensity", "Intensity");
		english.AddText("object-properties-light-component-radius", "Radius");
		english.AddText("object-properties-light-component-falloff", "Falloff");
		english.AddText("object-properties-light-component-cast-shadows", "Cast shadows");
		english.AddText("object-properties-light-component-soft-shadows", "Soft shadows");
		english.AddText("object-properties-light-component-source-size", "Source size");
		english.AddText("object-properties-light-component-dynamic-sky", "Dynamic Sky");

		english.AddText("object-properties-materials", "Materials");
		english.AddText("object-properties-unnamed-material", "Unnamed Material");

		// ImGui Widgets
		english.AddText("imgui-widgets-search-widget-placeholder", "Search...");

		s_Translations.push_back(english);
	}

	void Translations::Shutdown()
	{
		s_Translations.clear();
		s_Translations.shrink_to_fit();
	}

	void Translations::AddTranslation(const Translation &translation)
	{
		s_Translations.push_back(translation);
	}

	Translation *Translations::GetTranslation(const HLString &languageCode)
	{
		for (Translation &translation : s_Translations)
		{
			if (translation.GetLanguageCode() == languageCode)
			{
				return &translation;
			}
		}

		return nullptr;
	}

	bool Translations::HasTranslation(const HLString &languageCode)
	{
		for (Translation translation : s_Translations)
		{
			if (translation.GetLanguageCode() == languageCode)
			{
				return true;
			}
		}

		return false;
	}

	std::vector<Translation> &Translations::GetAllTranslations()
	{
		return s_Translations;
	}

	std::vector<HLString> Translations::GetAllLanguageCodes()
	{
		std::vector<HLString> result;
		for (Translation translation : s_Translations)
			result.push_back(translation.GetLanguageCode());

		return result;
	}

	std::vector<HLString> Translations::GetAllLanguages()
	{
		std::vector<HLString> result;
		for (Translation translation : s_Translations)
			result.push_back(translation.GetLanguageText());

		return result;
	}
}
