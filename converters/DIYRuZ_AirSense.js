const {
    fromZigbeeConverters,
    toZigbeeConverters,
} = require('zigbee-herdsman-converters');

const ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA = 0x0200; // non standart attribute, max precision
const ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE = 0x0200; //custom attribute for co2 readings
const ZCL_DATATYPE_UINT16 = 0x21;
const ZCL_DATATYPE_UINT32 = 0x23;




const CO2_LEVEL_KEY = ATTRID_MS_TEMPERATURE_MEASUREMENT_CO2_LEVEL_MEASURED_VALUE.toString();
const EXT_PRESSURE_KEY = ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA.toString();

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
    extended_pressure: {
        cluster: 'msPressureMeasurement',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            let pressure = 0;
            if (msg.data[EXT_PRESSURE_KEY]) {
                pressure = msg.data[EXT_PRESSURE_KEY] / 100.0;
            } else {
                pressure = msg.data.measuredValue;
            }
            return {
                pressure,
            };
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
        fromZigbeeConverters.humidity,
        fz.extended_pressure,
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
            'msTemperatureMeasurement',
            'msRelativeHumidity',
            'msPressureMeasurement'
        ]);

        const msBindPayload = [{
            attribute: 'measuredValue',
            minimumReportInterval: 0,
            maximumReportInterval: 3600,
            reportableChange: 0,
        }];

        const co2ReportingPayload = [
            ...msBindPayload,
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

        await firstEndpoint.configureReporting('msRelativeHumidity', msBindPayload);

        const pressureBindPayload = [
            ...msBindPayload,
            {
                attribute: {
                    ID: ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA,
                    type: ZCL_DATATYPE_UINT32,
                },
                minimumReportInterval: 0,
                maximumReportInterval: 3600,
                reportableChange: 0,
            },
        ];
        await firstEndpoint.configureReporting('msPressureMeasurement', pressureBindPayload);
    },
};

module.exports = device;