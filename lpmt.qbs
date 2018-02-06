import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/GUI.cpp",
            "src/GUI.hpp",
            "src/Util.cpp",
            "src/Util.hpp",
            "src/addons/ofRange.h",
            "src/addons/ofxMSAInteractiveObject.cpp",
            "src/addons/ofxMSAInteractiveObject.h",
            "src/addons/ofxMSATimer.cpp",
            "src/addons/ofxMSATimer.h",
            "src/addons/ofxTextInputField.cpp",
            "src/addons/ofxTextInputField.h",
            "src/appUtils.cpp",
            "src/config.h",
            "src/gui/Controls/ofxSimpleGuiButton.cpp",
            "src/gui/Controls/ofxSimpleGuiButton.h",
            "src/gui/Controls/ofxSimpleGuiColorPicker.cpp",
            "src/gui/Controls/ofxSimpleGuiColorPicker.h",
            "src/gui/Controls/ofxSimpleGuiComboBox.cpp",
            "src/gui/Controls/ofxSimpleGuiComboBox.h",
            "src/gui/Controls/ofxSimpleGuiContent.cpp",
            "src/gui/Controls/ofxSimpleGuiContent.h",
            "src/gui/Controls/ofxSimpleGuiFPSCounter.cpp",
            "src/gui/Controls/ofxSimpleGuiFPSCounter.h",
            "src/gui/Controls/ofxSimpleGuiMovieSlider.cpp",
            "src/gui/Controls/ofxSimpleGuiMovieSlider.h",
            "src/gui/Controls/ofxSimpleGuiQuadWarp.cpp",
            "src/gui/Controls/ofxSimpleGuiQuadWarp.h",
            "src/gui/Controls/ofxSimpleGuiSlider2d.cpp",
            "src/gui/Controls/ofxSimpleGuiSlider2d.h",
            "src/gui/Controls/ofxSimpleGuiSliderBase.h",
            "src/gui/Controls/ofxSimpleGuiSliderFloat.h",
            "src/gui/Controls/ofxSimpleGuiSliderInt.h",
            "src/gui/Controls/ofxSimpleGuiTitle.cpp",
            "src/gui/Controls/ofxSimpleGuiTitle.h",
            "src/gui/Controls/ofxSimpleGuiToggle.cpp",
            "src/gui/Controls/ofxSimpleGuiToggle.h",
            "src/gui/ofxSimpleGuiConfig.cpp",
            "src/gui/ofxSimpleGuiConfig.h",
            "src/gui/ofxSimpleGuiControl.cpp",
            "src/gui/ofxSimpleGuiControl.h",
            "src/gui/ofxSimpleGuiIncludes.h",
            "src/gui/ofxSimpleGuiPage.cpp",
            "src/gui/ofxSimpleGuiPage.h",
            "src/gui/ofxSimpleGuiToo.cpp",
            "src/gui/ofxSimpleGuiToo.h",
            "src/gui/ofxSimpleGuiValueControl.cpp",
            "src/gui/ofxSimpleGuiValueControl.h",
            "src/homography.cpp",
            "src/kinectManager.cpp",
            "src/kinectManager.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/parseMIDI.cpp",
            "src/parseOSC.cpp",
            "src/quad.cpp",
            "src/quad.h",
            "src/timeline.cpp",
            "src/timeline/ofOpenALSoundPlayer_TimelineAdditions.cpp",
            "src/timeline/ofOpenALSoundPlayer_TimelineAdditions.h",
            "src/timeline/ofxHotKeys.h",
            "src/timeline/ofxHotKeys_impl_linux.cpp",
            "src/timeline/ofxHotKeys_impl_win.cpp",
            "src/timeline/ofxTLAudioTrack.cpp",
            "src/timeline/ofxTLAudioTrack.h",
            "src/timeline/ofxTLBangs.cpp",
            "src/timeline/ofxTLBangs.h",
            "src/timeline/ofxTLCameraTrack.cpp",
            "src/timeline/ofxTLCameraTrack.h",
            "src/timeline/ofxTLColorTrack.cpp",
            "src/timeline/ofxTLColorTrack.h",
            "src/timeline/ofxTLColors.cpp",
            "src/timeline/ofxTLColors.h",
            "src/timeline/ofxTLCurves.cpp",
            "src/timeline/ofxTLCurves.h",
            "src/timeline/ofxTLEmptyKeyframes.cpp",
            "src/timeline/ofxTLEmptyKeyframes.h",
            "src/timeline/ofxTLEmptyTrack.cpp",
            "src/timeline/ofxTLEmptyTrack.h",
            "src/timeline/ofxTLEvents.h",
            "src/timeline/ofxTLFlags.cpp",
            "src/timeline/ofxTLFlags.h",
            "src/timeline/ofxTLImageSequence.cpp",
            "src/timeline/ofxTLImageSequence.h",
            "src/timeline/ofxTLImageSequenceFrame.cpp",
            "src/timeline/ofxTLImageSequenceFrame.h",
            "src/timeline/ofxTLImageTrack.cpp",
            "src/timeline/ofxTLImageTrack.h",
            "src/timeline/ofxTLInOut.cpp",
            "src/timeline/ofxTLInOut.h",
            "src/timeline/ofxTLKeyframes.cpp",
            "src/timeline/ofxTLKeyframes.h",
            "src/timeline/ofxTLLFO.cpp",
            "src/timeline/ofxTLLFO.h",
            "src/timeline/ofxTLPage.cpp",
            "src/timeline/ofxTLPage.h",
            "src/timeline/ofxTLPageTabs.cpp",
            "src/timeline/ofxTLPageTabs.h",
            "src/timeline/ofxTLSwitches.cpp",
            "src/timeline/ofxTLSwitches.h",
            "src/timeline/ofxTLTicker.cpp",
            "src/timeline/ofxTLTicker.h",
            "src/timeline/ofxTLTrack.cpp",
            "src/timeline/ofxTLTrack.h",
            "src/timeline/ofxTLTrackHeader.cpp",
            "src/timeline/ofxTLTrackHeader.h",
            "src/timeline/ofxTLVideoThumb.cpp",
            "src/timeline/ofxTLVideoThumb.h",
            "src/timeline/ofxTLVideoTrack.cpp",
            "src/timeline/ofxTLVideoTrack.h",
            "src/timeline/ofxTLZoomer.cpp",
            "src/timeline/ofxTLZoomer.h",
            "src/timeline/ofxTimeline.cpp",
            "src/timeline/ofxTimeline.h",
            "src/xmlSetup.cpp",
        ]

        of.addons: [
            'ofxKinect',
            'ofxMidi',
            'ofxNetwork',
            'ofxOpenCv',
            'ofxOsc',
            'ofxTimecode',
            'ofxTween',
            'ofxXmlSettings',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
