module.paths.push(require.main.path+ '/node_modules') //sovle Cannot find module 'zigbee-herdsman-converters'

const {
    fromZigbeeConverters,
    toZigbeeConverters,
    exposes
} = require('zigbee-herdsman-converters');
const ZCL_DATATYPE_INT16 = 0x29;
const ZCL_DATATYPE_UINT16 = 0x21;
const ZCL_DATATYPE_BOOLEAN = 0x10;
const ZCL_DATATYPE_INT32 = 0x2b;
const bind = async (endpoint, target, clusters) => {
    for (const cluster of clusters) {
        await endpoint.bind(cluster, target);
    }
};

const ACCESS_STATE = 0b001, ACCESS_WRITE = 0b010, ACCESS_READ = 0b100;

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
const deviceOptions = [{
        cluster: 'msCO2',
        attrId: 0x0203,
        type: ZCL_DATATYPE_BOOLEAN,
        key: 'led_feedback',
    },
    {
        cluster: 'msCO2',
        attrId: 0x0202,
        type: ZCL_DATATYPE_BOOLEAN,
        key: 'enable_abc',
    },
    {
        cluster: 'msCO2',
        attrId: 0x0204,
        type: ZCL_DATATYPE_UINT16,
        key: 'threshold1',
    },
    {
        cluster: 'msCO2',
        attrId: 0x0205,
        type: ZCL_DATATYPE_UINT16,
        key: 'threshold2',
    },
    {
        cluster: 'msTemperatureMeasurement',
        attrId: 0x0210,
        type: ZCL_DATATYPE_INT16,
        key: 'temperature_offset',
    },
    {
        cluster: 'msPressureMeasurement',
        attrId: 0x0210,
        type: ZCL_DATATYPE_INT32,
        key: 'pressure_offset',
    },
    {
        cluster: 'msRelativeHumidity',
        attrId: 0x0210,
        type: ZCL_DATATYPE_INT16,
        key: 'humidity_offset',
    },

];
const BOOL_MAP = ['OFF', 'ON'];
const generateConfigConverter = (cluster) => ({
    cluster,
    type: 'readResponse',
    convert: (model, msg, publish, options, meta) => {
        console.log(msg.data);
        const result = {};
        deviceOptions.forEach(({
            key,
            attrId,
            type
        }) => {
            if (attrId.toString() in msg.data) {
                if (type === ZCL_DATATYPE_BOOLEAN) {
                    result[key] = BOOL_MAP[msg.data[attrId.toString()]];
                } else {
                    result[key] = msg.data[attrId.toString()];
                }
            }
        });
        return result;
    }
});
const convertValue = (rawValue) => {
    const lookup = {
        'OFF': 0x00,
        'ON': 0x01,
    };
    return lookup.hasOwnProperty(rawValue) ? lookup[rawValue] : parseInt(rawValue, 10);
}
const tz = {
    config: {
        key: deviceOptions.map(({
            key
        }) => key),
        convertSet: async (entity, key, rawValue, meta) => {
            const {
                cluster,
                attrId,
                type
            } = deviceOptions.find(({
                key: _optionKey
            }) => key === _optionKey);

            await entity.write(cluster, {
                [attrId]: {
                    value: convertValue(rawValue),
                    type
                }
            });
            return {
                state: {
                    [key]: rawValue
                }
            };
        },
        convertGet: async (entity, key, meta) => {
            const {
                cluster,
                attrId
            } = deviceOptions.find(({
                key: _optionKey
            }) => key === _optionKey);
            await entity.read(cluster, [attrId]);
        },
    }
}

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
        fromZigbeeConverters.co2,
        fromZigbeeConverters.pressure,
        ...[...new Set(deviceOptions)].map(({
            cluster
        }) => generateConfigConverter(cluster))
    ],
    toZigbee: [
        toZigbeeConverters.factory_reset,
        tz.config
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

        const pressureBindPayload = [{
            attribute: 'scaledValue',
            minimumReportInterval: 0,
            maximumReportInterval: 3600,
            reportableChange: 0,
        }];
        await firstEndpoint.configureReporting('msPressureMeasurement', pressureBindPayload);
    },
    exposes: [
        exposes.numeric('co2', ACCESS_STATE).withUnit('ppm'),
        exposes.numeric('temperature', ACCESS_STATE).withUnit('°C'),
        exposes.numeric('humidity', ACCESS_STATE).withUnit('%'),
        exposes.numeric('pressure', ACCESS_STATE).withUnit('hPa'),


        //device options
        exposes.binary('led_feedback', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ, 'ON', 'OFF'),
        exposes.binary('enable_abc', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ, 'ON', 'OFF'),
        // led lights thresholds
        exposes.numeric('threshold1', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ).withUnit('ppm'),
        exposes.numeric('threshold2', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ).withUnit('ppm'),
        //fake BME280 workarounds
        exposes.numeric('temperature_offset', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ).withUnit('°C'),
        exposes.numeric('pressure_offset', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ).withUnit('hPa'),
        exposes.numeric('humidity_offset', ACCESS_STATE | ACCESS_WRITE | ACCESS_READ).withUnit('%')
    ],
};

module.exports = device;
