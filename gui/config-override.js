const webpack = require('webpack');

module.exports = (config, env) => {

    config.plugins.push(
        new webpack.ProvidePlugin({
            React: 'react',
            ReactDOM: 'react-dom',
            $: 'jquery',
            jQuery: 'jquery',
            JQuery: 'jquery'
        }));

    return config;
};
