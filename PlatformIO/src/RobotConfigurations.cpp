// RBotFirmware
// Rob Dobson 2016-2018

#include "RobotConfigurations.h"

const char *RobotConfigurations::_robotConfigs[] = {

"{"
" \"robotType\": \"SandTableScaraPiHat2\","
" \"cmdsAtStart\": \"\","
" \"webui\":\"SandUI\","
" \"evaluators\":"
" {"
"   \"thrStepDegs\": 2.8,"
"   \"thrContinue\": 0"
" }"
" ,"
" \"robotGeom\":"
" {"
"   \"model\": \"SingleArmScara\","
"   \"homing\":"
"   {"
"     \"homingSeq\": \"A-10000n;B10000;#;A+10000N;B-10000;#;A+500;B-500;#;B+10000n;#;B-10000N;#;B-1050;#;A=h;B=h;$\","
"     \"maxHomingSecs\": 120"
"   }"
"   ,"
"   \"blockDistanceMM\": 1,"
"   \"allowOutOfBounds\": 0,"
"   \"stepEnablePin\": \"4\","
"   \"stepEnLev\": 1,"
"   \"stepDisableSecs\": 10,"
"   \"axis0\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"stepPin\": \"14\","
"     \"dirnPin\": \"13\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"36\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
"   ,"
"   \"axis1\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"stepPin\": \"15\","
"     \"dirnPin\": \"21\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"39\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
" }"
" ,"
" \"fileManager\":"
" {"
"   \"spiffsEnabled\": 1,"
"   \"spiffsFormatIfCorrupt\": 1,"
"   \"sdEnabled\": 1,"
"   \"sdMOSI\": \"23\","
"   \"sdMISO\": \"19\","
"   \"sdCLK\": \"18\","
"   \"sdCS\": \"4\""
" }"
" ,"
" \"wifiLed\":"
" {"
"   \"hwPin\": \"14\","
"   \"onLevel\": 1,"
"   \"onMs\": 200,"
"   \"shortOffMs\": 200,"
"   \"longOffMs\": 750"
" }"
" ,"
" \"ledStrip\":"
" {"
"   \"ledPin\": \"32\","
"   \"sensorPin\": \"33\""
" }"
"}"
,
"{"
" \"robotType\": \"SandTableScaraPiHat3.6\","
" \"cmdsAtStart\": \"\","
" \"webui\":\"SandUI\","
" \"evaluators\":"
" {"
"   \"thrStepDegs\": 2.8,"
"   \"thrContinue\": 0"
" }"
" ,"
" \"robotGeom\":"
" {"
"   \"model\": \"SingleArmScara\","
"   \"homing\":"
"   {"
"     \"homingSeq\": \"A-10000n;B10000;#;A+10000N;B-10000;#;A+500;B-500;#;B+10000n;#;B-10000N;#;B-1050;#;A=h;B=h;$\","
"     \"maxHomingSecs\": 120"
"   }"
"   ,"
"   \"blockDistanceMM\": 1,"
"   \"allowOutOfBounds\": 0,"
"   \"stepEnablePin\": \"21\","
"   \"stepEnLev\": 0,"
"   \"stepDisableSecs\": 10,"
"   \"axis0\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"stepPin\": \"27\","
"     \"dirnPin\": \"33\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"39\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
"   ,"
"   \"axis1\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"stepPin\": \"12\","
"     \"dirnPin\": \"16\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"36\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
" }"
" ,"
" \"fileManager\":"
" {"
"   \"spiffsEnabled\": 1,"
"   \"spiffsFormatIfCorrupt\": 1,"
"   \"sdEnabled\": 1,"
"   \"sdMOSI\": \"23\","
"   \"sdMISO\": \"19\","
"   \"sdCLK\": \"18\","
"   \"sdCS\": \"4\""
" }"
" ,"
" \"wifiLed\":"
" {"
"   \"hwPin\": \"14\","
"   \"onLevel\": 1,"
"   \"onMs\": 200,"
"   \"shortOffMs\": 200,"
"   \"longOffMs\": 750"
" }"
" ,"
" \"ledStrip\":"
" {"
"   \"ledPin\": \"25\","
"   \"sensorPin\": \"-1\""
" }"
"}"
,
"{"
" \"robotType\": \"SandTableScaraPiHat4\","
" \"cmdsAtStart\": \"\","
" \"webui\":\"SandUI\","
" \"evaluators\":"
" {"
"   \"thrStepDegs\": 2.8,"
"   \"thrContinue\": 0"
" }"
" ,"
" \"robotGeom\":"
" {"
"   \"model\": \"SingleArmScara\","
"   \"homing\":"
"   {"
"     \"homingSeq\": \"A-10000n;B10000;#;A+10000N;B-10000;#;A+500;B-500;#;B+10000n;#;B-10000N;#;B-1050;#;A=h;B=h;$\","
"     \"maxHomingSecs\": 120"
"   }"
"   ,"
"   \"blockDistanceMM\": 1,"
"   \"allowOutOfBounds\": 0,"
"   \"axis0\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"unitsPerRot\": 628.318,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"36\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
"   ,"
"   \"axis1\":"
"   {"
"     \"maxSpeed\": 75,"
"     \"maxAcc\": 150,"
"     \"stepsPerRot\": 9600,"
"     \"unitsPerRot\": 628.318,"
"     \"maxRPM\": 60,"
"     \"maxVal\": 92.5,"
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"39\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
" }"
" ,"
" \"fileManager\":"
" {"
"   \"spiffsEnabled\": 1,"
"   \"spiffsFormatIfCorrupt\": 1,"
"   \"sdEnabled\": 1,"
"   \"sdMOSI\": \"23\","
"   \"sdMISO\": \"19\","
"   \"sdCLK\": \"18\","
"   \"sdCS\": \"4\""
" }"
" ,"
" \"wifiLed\":"
" {"
"   \"hwPin\": \"14\","
"   \"onLevel\": 1,"
"   \"onMs\": 200,"
"   \"shortOffMs\": 200,"
"   \"longOffMs\": 750"
" }"
"}"
,
"{"
" \"robotType\": \"SandTableScaraMatt\","
" \"cmdsAtStart\": \"g28\","
" \"webui\":\"SandUI\","
" \"evaluators\":"
" {"
"   \"thrStepDegs\": 2.8,"
"   \"thrContinue\": 0"
" }"
" ,"
" \"robotGeom\":"
" {"
"   \"model\": \"SingleArmScara\","
"   \"homing\":"
"   {"
"     \"homingSeq\":\"A-10000S2N;B+10000S2#;A-10000S2n;B+10000S2;#;A+470S2;B-470S2;#;B-10000S2N;#;B-10000S2n;#;B+850S2;#;A=h;B=h;$\","
"     \"maxHomingSecs\": 120"
"   }"
"   ,"
"   \"blockDistanceMM\": 1,"
"   \"allowOutOfBounds\": 0,"
"   \"stepEnablePin\": \"12\","
"   \"stepEnLev\": 0,"
"   \"stepDisableSecs\": 10,"
"   \"axis0\":"
"   {"
"     \"maxSpeed\": 10,"
"     \"maxAcc\": 50,"
"     \"stepsPerRot\": 9600,"
"     \"unitsPerRot\": 628.318,"
"     \"maxVal\": 190,"
"     \"stepPin\": \"14\","
"     \"dirnPin\": \"32\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"36\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
"   ,"
"   \"axis1\":"
"   {"
"     \"maxSpeed\": 10,"
"     \"maxAcc\": 50,"
"     \"stepsPerRot\": 9600,"
"     \"unitsPerRot\": 628.318,"
"     \"maxVal\": 190,"
"     \"stepPin\": \"33\","
"     \"dirnPin\": \"15\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"39\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
" }"
" ,"
" \"fileManager\":"
" {"
"   \"spiffsEnabled\": 1,"
"   \"spiffsFormatIfCorrupt\": 1,"
"   \"sdEnabled\": 1,"
"   \"sdMOSI\": \"18\","
"   \"sdMISO\": \"19\","
"   \"sdCLK\": \"5\","
"   \"sdCS\": \"21\""
" }"
" ,"
" \"wifiLed\":"
" {"
"   \"hwPin\": \"14\","
"   \"onLevel\": 1,"
"   \"onMs\": 200,"
"   \"shortOffMs\": 200,"
"   \"longOffMs\": 750"
" }"
" ,"
" \"ledStrip\":"
" {"
"   \"ledPin\": \"4\","
"   \"sensorPin\": \"34\""
" }"
"}"
,
"{"
" \"robotType\": \"XYBot\","
" \"cmdsAtStart\": \"\","
" \"webui\":\"cncUI\","
" \"robotGeom\":"
" {"
"   \"model\": \"Cartesian\","
"   \"blockDistanceMM\": 1,"
"   \"allowOutOfBounds\": 0,"
"   \"stepEnablePin\": \"21\","
"   \"stepEnLev\": 0,"
"   \"stepDisableSecs\": 10,"
"   \"axis0\":"
"   {"
"     \"maxSpeed\": 50,"
"     \"maxAcc\": 50,"
"     \"stepsPerRot\": 3200,"
"     \"unitsPerRot\": 16,"
"     \"maxRPM\": 200,"
"     \"maxVal\": 200,"
"     \"stepPin\": \"27\","
"     \"dirnPin\": \"33\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"39\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
"   ,"
"   \"axis1\":"
"   {"
"     \"maxSpeed\": 50,"
"     \"maxAcc\": 50,"
"     \"stepsPerRot\": 3200,"
"     \"unitsPerRot\": 16,"
"     \"maxRPM\": 200,"
"     \"maxVal\": 200,"
"     \"stepPin\": \"12\","
"     \"dirnPin\": \"16\","
"     \"endStop0\":"
"     {"
"       \"sensePin\": \"36\","
"       \"actLvl\": 0,"
"       \"inputType\": \"INPUT_PULLUP\""
"     }"
"   }"
" }"
" ,"
" \"fileManager\":"
" {"
"   \"spiffsEnabled\": 1,"
"   \"spiffsFormatIfCorrupt\": 1,"
"   \"sdEnabled\": 1,"
"   \"sdMOSI\": \"23\","
"   \"sdMISO\": \"19\","
"   \"sdCLK\": \"18\","
"   \"sdCS\": \"4\""
" }"
" ,"
" \"wifiLed\":"
" {"
"   \"hwPin\": \"14\","
"   \"onLevel\": 1,"
"   \"onMs\": 200,"
"   \"shortOffMs\": 200,"
"   \"longOffMs\": 750"
" }"
"}"

//     "{"
//     "\"robotType\":\"MugBot\","
//     "\"robotGeom\":\"MugBot\","
//     "\"robotBoard\":\"PiHat3\","
//     "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
//             "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
//     "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
//     "\"homingSeq\":\"B-x;B+r9X;B-1.0;B=h$\","
//     "\"cmdsAtStart\":\"\","
//     "\"stepEnablePin\":\"A2\",\"stepEnLev\":1,\"stepDisableSecs\":60.0,"
//     "\"axis0\":{\"stepPin\":\"D2\",\"dirnPin\":\"D3\",\"maxSpeed\":10.0,\"maxAcc\":5.0,"
//     "\"stepsPerRot\":6400,\"unitsPerRot\":360,\"minVal\":-360,\"maxVal\":360},"
//     "\"axis1\":{\"stepPin\":\"D4\",\"dirnPin\":\"D5\",\"maxSpeed\":5.0,\"maxAcc\":5.0,"
//     "\"stepsPerRot\":3200,\"unitsPerRot\":2.0,\"minVal\":0,\"maxVal\":100,"
//     "\"homeOffsetVal\":100,"
//     "\"endStop1\":{\"sensePin\":\"A7\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}},"
//     "\"axis2\":{\"servoPin\":\"D0\",\"isServoAxis\":1,\"isPrimaryAxis\": 0,\"homeOffsetVal\":180,\"homeOffSteps\":2500,"
//     "\"minVal\":0,\"maxVal\":180,\"stepsPerRot\":2000,\"unitsPerRot\":360}"
//     "}",

//     "{"
//     "\"robotType\":\"GeistBot\","
//     "\"robotGeom\":\"GeistBot\","
//     "\"robotBoard\":\"PiHat3\","
    //     "{\"robotType\":\"GeistBot\","
    // "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
    //         "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
    // "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
    //     " \"stepEnablePin\":\"A2\",\"stepEnLev\":1,\"stepDisableSecs\":1.0,"
    //     " \"maxHomingSecs\":120,\"homingLinOffsetDegs\":70,\"homingCentreOffsetMM\":4,"
    //     " \"homingRotCentreDegs\":3.7,\"cmdsAtStart\":\"G28;ModSpiral\", "
    //     " \"axis0\": { \"stepPin\":\"D2\",\"dirnPin\":\"D3\",\"maxSpeed\":75.0,\"maxAcc\":5.0,"
    //     " \"stepsPerRot\":12000,\"unitsPerRot\":360,\"isDominantAxis\":1,"
    //     " \"endStop0\": { \"sensePin\":\"A6\",\"actLvl\":1,\"inputType\":\"INPUT_PULLUP\"}},"
    //     " \"axis1\": { \"stepPin\":\"D4\",\"dirnPin\":\"D5\",\"maxSpeed\":75.0,\"maxAcc\":5.0,"
    //     " \"stepsPerRot\":12000,\"unitsPerRot\":44.8,\"minVal\":0,\"maxVal\":195, "
    //     " \"endStop0\": { \"sensePin\":\"A7\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}},"
    //     "}",

    // "{"
    // "\"robotType\":\"SandTableScaraPiHat3\","
    // "\"robotGeom\":\"SingleArmScara\","
    // "\"robotBoard\":\"PiHat3\","
    // "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
    //         "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
    // "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
    // "\"cmdsAtStart\":\"\","
    // "\"homingSeq\":\"A-10000n;B10000;#;A+10000N;B-10000;#;A+500;B-500;#;B+10000n;#;B-10000N;#;B-1050;#;A=h;B=h;$\","
    // // "\"homingSeq\":\"A-10000n;B10000;#;A=h;B=h;$\","
    // "\"maxHomingSecs\":120,"
    // "\"stepEnablePin\":\"4\",\"stepEnLev\":1,\"stepDisableSecs\":10,"
    // "\"blockDistanceMM\":1,\"allowOutOfBounds\":1,"
    // "\"axis0\":{\"stepPin\":\"14\",\"dirnPin\":\"13\",\"maxSpeed\":75,\"maxAcc\":50,"
    // "\"stepsPerRot\":9600,\"unitsPerRot\":628.318,\"maxVal\":185,"
    // "\"endStop0\":{\"sensePin\":\"36\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}},"
    // "\"axis1\":{\"stepPin\":\"15\",\"dirnPin\":\"21\",\"maxSpeed\":75,\"maxAcc\":50,"
    // "\"stepsPerRot\":9600,\"unitsPerRot\":628.318,\"maxVal\":185,"
    // "\"endStop0\":{\"sensePin\":\"39\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}}"
    // "}",

//     "{"
//     "\"robotType\":\"SandTableScaraPiHat4\","
//     "\"robotGeom\":\"SingleArmScara\","
//     "\"robotBoard\":\"PiHat4\","
//     "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
//             "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
//     "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
//     "\"cmdsAtStart\":\"\","
//     "\"homingSeq\":\"A-10000n;B10000;#;A+10000N;B-10000;#;A+500;B-500;#;B+10000n;#;B-10000N;#;B-1050;#;A=h;B=h;$\","
//     "\"maxHomingSecs\":120,"
//     // "\"stepEnablePin\":\"4\",\"stepEnLev\":1,\"stepDisableSecs\":10,"
//     "\"blockDistanceMM\":1,\"allowOutOfBounds\":1,"
//     "\"axis0\":{\"tmc5072channel\":\"0\",\"maxSpeed\":75,\"maxAcc\":50,"
//     "\"stepsPerRot\":9600,\"unitsPerRot\":628.318,\"maxVal\":185,"
//     "\"endStop0\":{\"sensePin\":\"36\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}},"
//     "\"axis1\":{\"tmc5072channel\":\"1\",\"maxSpeed\":75,\"maxAcc\":50,"
//     "\"stepsPerRot\":9600,\"unitsPerRot\":628.318,\"maxVal\":185,"
//     "\"endStop0\":{\"sensePin\":\"39\",\"actLvl\":0,\"inputType\":\"INPUT_PULLUP\"}}"
//     "}",

//     "{"
//     "\"robotType\":\"HockeyBot\","
//     "\"robotGeom\":\"HBot\","
//     "\"robotBoard\":\"PiHat3\","
    // "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
    //         "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
    // "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
    //     " \"stepEnablePin\":\"A2\",\"stepEnLev\":1,\"stepDisableSecs\":1.0,"
    //     " \"cmdsAtStart\":\"\", "
    //     " \"axis0\": { \"stepPin\":\"D2\",\"dirnPin\":\"D3\",\"maxSpeed\":5000.0,\"maxAcc\":5000.0,"
    //     " \"stepsPerRot\":3200,\"unitsPerRot\":62},"
    //     " \"axis1\": { \"stepPin\":\"D4\",\"dirnPin\":\"D5\",\"maxSpeed\":5000.0,\"maxAcc\":5000.0,"
    //     " \"stepsPerRot\":3200,\"unitsPerRot\":62}"
    //     "}",

//     "{"
//     "\"robotType\":\"XYBot\","
//     "\"robotGeom\":\"Cartesian\","
//     "\"robotBoard\":\"PiHat3\","
//     "\"fileManager\":{\"spiffsEnabled\":1,\"spiffsFormatIfCorrupt\":1,"
//             "\"sdEnabled\":1,\"sdMOSI\":\"23\",\"sdMISO\":\"19\",\"sdCLK\":\"18\",\"sdCS\":\"4\"},"
//     "\"wifiLed\":{\"hwPin\":\"14\",\"onLevel\":1,\"onMs\":200,\"shortOffMs\":200,\"longOffMs\":750},"
//     "\"stepEnablePin\":\"A2\",\"stepEnLev\":1,\"stepDisableSecs\":1.0,"
//     "\"cmdsAtStart\":\"\", "
//     "\"axis0\":{\"stepPin\":\"D2\",\"dirnPin\":\"D3\",\"maxSpeed\":100.0,\"maxAcc\":10.0,"
//     "\"stepsPerRot\":3200,\"unitsPerRot\":32},"
//     "\"axis1\":{\"stepPin\":\"D4\",\"dirnPin\":\"D5\",\"maxSpeed\":100.0,\"maxAcc\":10.0,"
//     "\"stepsPerRot\":3200,\"unitsPerRot\":32},"
//     "\"workItemQueue\":{\"maxLen\":50}"
//     "}"
    };

const int RobotConfigurations::_numRobotConfigurations = sizeof(RobotConfigurations::_robotConfigs) / sizeof(const char *);
