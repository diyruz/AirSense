const {
    fromZigbeeConverters,
    toZigbeeConverters,
} = require('zigbee-herdsman-converters');

const ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA = 0x0200; // non standart attribute, max precision
const ZCL_DATATYPE_UINT32 = 0x23;
const EXT_PRESSURE_KEY = ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE_HPA.toString();

const bind = async (endpoint, target, clusters) => {
    for (const cluster of clusters) {
        await endpoint.bind(cluster, target);
    }
};

const fz = {
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

const hass = {
    co2: {
        type: 'sensor',
        object_id: 'co2',
        discovery_payload: {
            unit_of_measurement: 'ppm',
            icon: 'mdi:molecule-co2',
            value_template: '{{ value_json.co2 }}',
        },
    },
    temperature: {
        type: 'sensor',
        object_id: 'temperature',
        discovery_payload: {
            unit_of_measurement: '°C',
            device_class: 'temperature',
            value_template: '{{ value_json.temperature }}',
        },
    },
    humidity: {
        type: 'sensor',
        object_id: 'humidity',
        discovery_payload: {
            unit_of_measurement: '%',
            device_class: 'humidity',
            value_template: '{{ value_json.humidity }}',
        },
    },
    presure: {
        type: 'sensor',
        object_id: 'pressure',
        discovery_payload: {
            unit_of_measurement: 'hPa',
            device_class: 'pressure',
            value_template: '{{ value_json.pressure }}',
        },
    }
};

const device = {
    zigbeeModel: ['DIYRuZ_AirSense'],
    model: 'DIYRuZ_AirSense',
    vendor: 'DIYRuZ',
    description: '[Air quality sensor](http://modkam.ru/?p=xxxx)',
    supports: '',
    homeassistant: [hass.temperature, hass.presure, hass.humidity, hass.co2],
    fromZigbee: [
        fromZigbeeConverters.temperature,
        fromZigbeeConverters.humidity,
        fromZigbeeConverters.co2
        fz.extended_pressure,
    ],
    toZigbee: [
        toZigbeeConverters.factory_reset,
    ],
    meta: {
        configureKey: 1,
    },
    configure: async (device, coordinatorEndpoint) => {
        const firstEndpoint = device.getEndpoint(1);

        await bind(firstEndpoint, coordinatorEndpoint, [
            'msTemperatureMeasurement',
            'msRelativeHumidity',
            'msPressureMeasurement',
            'msCO2'
        ]);

        const msBindPayload = [{
            attribute: 'measuredValue',
            minimumReportInterval: 0,
            maximumReportInterval: 3600,
            reportableChange: 0,
        }];

        await firstEndpoint.configureReporting('msCO2', msBindPayload);
        await firstEndpoint.configureReporting('msTemperatureMeasurement', msBindPayload);
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