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
            "src/appUtils.cpp",
            "src/config.h",
            "src/homography.cpp",
            "src/kinectManager.cpp",
            "src/kinectManager.h",
            "src/main.cpp",
            "src/markers.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/parseMIDI.cpp",
            "src/parseOSC.cpp",
            "src/quad.cpp",
            "src/quad.h",
            "src/timeline.cpp",
            "src/xmlSetup.cpp",
        ]

        of.addons: [
            'ofxKinect',
            'ofxMidi',
            'ofxMSAInteractiveObjectLPMT',
            'ofxMSATimer',
            'ofxNetwork',
            'ofxOpenCv',
            'ofxOsc',
            'ofxRange',
            'ofxSimpleGuiTooLPMT',
            'ofxTextInputField',
            'ofxTimecode',
            'ofxTween',
            'ofxXmlSettings',
            'ofxMostPixelsEverLPMT',
            'ofxTimelineLPMT',
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
