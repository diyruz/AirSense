const {
    fromZigbeeConverters,
    toZigbeeConverters,
} = require('zigbee-herdsman-converters');


const ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE = 0x0200; //custom attribute for co2 readings
const ZCL_DATATYPE_UINT16 = 0x21;
const ZCL_DATATYPE_UINT32 = 0x23;




const CO2_LEVEL_KEY = ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE.toString();

const bind = async (endpoint, target, clusters) => {
    for (const cluster of clusters) {
        await endpoint.bind(cluster, target);
    }
};


const fz = {
    co2: {
        cluster: 'msTemperatureMeasurement',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            if (msg.data[CO2_LEVEL_KEY]) {
                return {co2: msg.data[CO2_LEVEL_KEY]};
            }
        },
    },
};

const device = {
    zigbeeModel: ['DIYRuZ_AirSense'],
    model: 'DIYRuZ_AirSense',
    vendor: 'DIYRuZ',
    description: '[Air quality sensor](http://modkam.ru/?p=xxxx)',
    supports: '',
    fromZigbee: [
        fromZigbeeConverters.temperature,
        fz.co2
    ],
    toZigbee: [
        toZigbeeConverters.factory_reset,
    ],
    meta: {
        configureKey: 1,
        disableDefaultResponse: true,
    },
    configure: async (device, coordinatorEndpoint) => {
        const firstEndpoint = device.getEndpoint(1);

        await bind(firstEndpoint, coordinatorEndpoint, [
            'msTemperatureMeasurement'
        ]);

        const co2ReportingPayload = [{
                attribute: 'measuredValue',
                minimumReportInterval: 0,
                maximumReportInterval: 3600,
                reportableChange: 0,
            },
            {
                attribute: {
                    ID: ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE,
                    type: ZCL_DATATYPE_UINT16,
                },
                minimumReportInterval: 0,
                maximumReportInterval: 3600,
                reportableChange: 0,
            },
        ];
        await firstEndpoint.configureReporting('msTemperatureMeasurement', co2ReportingPayload);

    },
};

module.exports = device;