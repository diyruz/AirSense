const {
    fromZigbeeConverters,
    toZigbeeConverters,
    exposes
} = require('zigbee-herdsman-converters');

const bind = async (endpoint, target, clusters) => {
    for (const cluster of clusters) {
        await endpoint.bind(cluster, target);
    }
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
const deviceOptions = [{
    cluster: 'msCO2',
    attrId: 0x0203,
    type: 0x10,
    key: 'led_feedback',
}];
const generateConfigConverter = (cluster) => ({
    cluster,
    type: 'readResponse',
    convert: (model, msg, publish, options, meta) => {
        const result = {};
        deviceOptions.forEach(({
            key,
            attrId,
            type
        }) => {
            if (attrId.toString() in msg.data) {
                result[key] = msg.data[attrId.toString()];
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
            const { cluster, attrId, type } = deviceOptions.find(({key: _optionKey}) => key === _optionKey);

            await entity.write(cluster, {
                [attrId]: {
                    value: convertValue(rawValue),
                    type
                }
            });
            return {state: { [key]: rawValue}};
        },
        convertGet: async (entity, key, meta) => {
            const { cluster, attrId } = deviceOptions.find(({key: _optionKey}) => key === _optionKey);
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
        ...[...new Set(deviceOptions)].map(({cluster}) => generateConfigConverter(cluster))
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
        exposes.numeric('co2').withUnit('ppm'),
        exposes.numeric('temperature').withUnit('°C'),
        exposes.numeric('humidity').withUnit('%'),
        exposes.numeric('pressure').withUnit('hPa'),
        exposes.switch('led_feedback'),
    ],
};

module.exports = device;