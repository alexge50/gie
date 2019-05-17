const webpack = require('webpack');
const ModuleScopePlugin = require('react-dev-utils/ModuleScopePlugin');
const ElectronNativePlugin = require('electron-native-plugin');

module.exports = (config, env) => {

    config.plugins.push(
        new webpack.ProvidePlugin({
            React: 'react',
            ReactDOM: 'react-dom',
            $: 'jquery',
            jQuery: 'jquery',
            JQuery: 'jquery'
        }));

    config.resolve.plugins = config.resolve.plugins.filter(plugin => !(plugin instanceof ModuleScopePlugin));

    config.externals = {
      'gie_nodejs': "require('./build/Release/gie_nodejs')",
      'fs': "require('fs')",
    };

    config.target = "electron-renderer";

    config.plugins.push(new ElectronNativePlugin());
    config.plugins.push(new webpack.IgnorePlugin(/node-gyp/));

    config.module.rules.push({
        test: /\.js$/,
        use:
            [
                "electron-native-patch-loader",
                {
                    loader: "electron-native-loader",
                    options: {
                        outputPath: "./dist"
                    }
                }
            ]
    });

    config.module.rules.push({
            test: /\.node$/,
            use: "electron-native-loader"
    });

    config.resolve.alias["gie"] = "build/Release/gie.node";

    return config;
};
