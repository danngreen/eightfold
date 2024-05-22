#include "plugin.hpp"
#include "SDSharedComponents.hpp"

#define CHANNEL_COUNT 8

struct SDTransgate : Module {
    enum ParamId {
        TRANS1_PARAM,
        TRANS2_PARAM,
        TRANS3_PARAM,
        TRANS4_PARAM,
        TRANS5_PARAM,
        TRANS6_PARAM,
        TRANS7_PARAM,
        TRANS8_PARAM,
        ALL_ON_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        GATE1_INPUT,
        GATE2_INPUT,
        GATE3_INPUT,
        GATE4_INPUT,
        GATE5_INPUT,
        GATE6_INPUT,
        GATE7_INPUT,
        GATE8_INPUT,
        GATES_INPUT,
        INPUT_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        OUTPUT_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        GATEON1_LIGHT,
        GATEON2_LIGHT,
        GATEON3_LIGHT,
        GATEON4_LIGHT,
        GATEON5_LIGHT,
        GATEON6_LIGHT,
        GATEON7_LIGHT,
        GATEON8_LIGHT,
        LIGHTS_LEN
    };

    SDTransgate() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

        configParam(TRANS1_PARAM, -12.f, 12.f, 0.f, "Transpose 1");
        configParam(TRANS2_PARAM, -12.f, 12.f, 0.f, "Transpose 2");
        configParam(TRANS3_PARAM, -12.f, 12.f, 0.f, "Transpose 3");
        configParam(TRANS4_PARAM, -12.f, 12.f, 0.f, "Transpose 4");
        configParam(TRANS5_PARAM, -12.f, 12.f, 0.f, "Transpose 5");
        configParam(TRANS6_PARAM, -12.f, 12.f, 0.f, "Transpose 6");
        configParam(TRANS7_PARAM, -12.f, 12.f, 0.f, "Transpose 7");
        configParam(TRANS8_PARAM, -12.f, 12.f, 0.f, "Transpose 8");
        
        for (int i = 0; i < CHANNEL_COUNT; i++) {
            getParamQuantity(TRANS1_PARAM + i)->snapEnabled = true;
        }

        configLight(GATEON1_LIGHT, "Gate 1 On");
        configLight(GATEON2_LIGHT, "Gate 2 On");
        configLight(GATEON3_LIGHT, "Gate 3 On");
        configLight(GATEON4_LIGHT, "Gate 4 On");
        configLight(GATEON5_LIGHT, "Gate 5 On");
        configLight(GATEON6_LIGHT, "Gate 6 On");
        configLight(GATEON7_LIGHT, "Gate 7 On");
        configLight(GATEON8_LIGHT, "Gate 8 On");

        configSwitch(ALL_ON_PARAM, 0.0, 1.0, 0.0, "All Gates On", {"On", "Off"});
        
        configInput(GATE1_INPUT, "Gate 1");
        configInput(GATE2_INPUT, "Gate 2");
        configInput(GATE3_INPUT, "Gate 3");
        configInput(GATE4_INPUT, "Gate 4");
        configInput(GATE5_INPUT, "Gate 5");
        configInput(GATE6_INPUT, "Gate 6");
        configInput(GATE7_INPUT, "Gate 7");
        configInput(GATE8_INPUT, "Gate 8");

        configInput(GATES_INPUT, "Gates");
        configInput(INPUT_INPUT, "Input");
        configOutput(OUTPUT_OUTPUT, "Output");
    }

    void process(const ProcessArgs& args) override {
        int gate_count = inputs[GATES_INPUT].getChannels();
        int input_count = inputs[INPUT_INPUT].getChannels();
        if (input_count > CHANNEL_COUNT) input_count = CHANNEL_COUNT;
        
        bool gates[CHANNEL_COUNT];
        bool last_gate = gate_count > 0 ? inputs[GATES_INPUT].getVoltage(gate_count - 1) : false;

        if (params[ALL_ON_PARAM].getValue() == 1) {
            for (int i = 0; i < CHANNEL_COUNT; i++) {
                gates[i] = true;
                lights[GATEON1_LIGHT + i].setBrightness(1.f);
            }
        } else {
            for (int i = 0; i < CHANNEL_COUNT; i++) {
                gates[i] = inputs[GATE1_INPUT + i].getChannels() ?
                    inputs[GATE1_INPUT + i].getVoltage() > 0 :
                    i < gate_count ? inputs[GATES_INPUT].getVoltage(i) > 0 : last_gate;
                lights[GATEON1_LIGHT + i].setBrightness(gates[i] ? 1.f : 0.f);
            }
        }
        
        for (int i = 0; i < input_count; i++) {
            float voltage = inputs[INPUT_INPUT].getVoltage(i);
            if (gates[i]) voltage += params[TRANS1_PARAM + i].getValue() / 12.f;
            outputs[OUTPUT_OUTPUT].setVoltage(voltage, i);
        }
        
        outputs[OUTPUT_OUTPUT].setChannels(input_count);
    }
};


struct SDTransgateWidget : ModuleWidget {
    SDTransgateWidget(SDTransgate* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/SDTransgate.svg")));

        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 13.126)), module, SDTransgate::TRANS1_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 24.148)), module, SDTransgate::TRANS2_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 35.17)), module, SDTransgate::TRANS3_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 46.192)), module, SDTransgate::TRANS4_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 57.214)), module, SDTransgate::TRANS5_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 68.236)), module, SDTransgate::TRANS6_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 79.258)), module, SDTransgate::TRANS7_PARAM));
        addParam(createParamCentered<SDSmallestKnob>(mm2px(Vec(17.836, 90.28)), module, SDTransgate::TRANS8_PARAM));
        addParam(createParam<SDTwoSwitch>(mm2px(Vec(15.617, 101.089)), module, SDTransgate::ALL_ON_PARAM));

        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 13.126)), module, SDTransgate::GATE1_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 24.148)), module, SDTransgate::GATE2_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 35.17)), module, SDTransgate::GATE3_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 46.192)), module, SDTransgate::GATE4_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 57.214)), module, SDTransgate::GATE5_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 68.236)), module, SDTransgate::GATE6_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 79.258)), module, SDTransgate::GATE7_INPUT));
        addInput(createInputCentered<SDMonoInPort>(mm2px(Vec(6.652, 90.28)), module, SDTransgate::GATE8_INPUT));
        addInput(createInputCentered<SDPolyInPort>(mm2px(Vec(6.652, 104.61)), module, SDTransgate::GATES_INPUT));
        addInput(createInputCentered<SDPolyInPort>(mm2px(Vec(6.652, 119.319)), module, SDTransgate::INPUT_INPUT));

        addOutput(createOutputCentered<SDPolyOutPort>(mm2px(Vec(17.836, 119.318)), module, SDTransgate::OUTPUT_OUTPUT));

        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 13.126)), module, SDTransgate::GATEON1_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 24.148)), module, SDTransgate::GATEON2_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 35.17)), module, SDTransgate::GATEON3_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 46.192)), module, SDTransgate::GATEON4_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 57.214)), module, SDTransgate::GATEON5_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 68.236)), module, SDTransgate::GATEON6_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 79.258)), module, SDTransgate::GATEON7_LIGHT));
        addChild(createLightCentered<SmallLight<SDWhiteLight>>(mm2px(Vec(12.054, 90.28)), module, SDTransgate::GATEON8_LIGHT));
    }
};


Model* modelSDTransgate = createModel<SDTransgate, SDTransgateWidget>("SDTransgate");