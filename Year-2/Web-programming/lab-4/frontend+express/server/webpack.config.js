const path = require('path');
const nodeExternals = require('webpack-node-externals');
const LoadablePlugin = require('@loadable/webpack-plugin')
const ringConfig = require('@jetbrains/ring-ui/webpack.config').config;

process.env.NODE_ENV = 'production';

const webpackConfig = {
    mode: 'development' === process.env.NODE_ENV ? 'development' : 'production',

    entry: path.resolve('server/index.js'),
    output: {
        path: path.resolve('dist/server'),
        filename: 'index.js',
    },

    target: 'node',

    externals: [nodeExternals()], // in order to ignore all modules in node_modules folder
    externalsPresets: {
        node: true // in order to ignore built-in modules like path, fs, etc. 
    },

    module: {
        rules: [
            ...ringConfig.module.rules,
            {
                test: /\.(png|jp(e*)g|gif)$/,
                use: ['file-loader'],
            },
            {
                test: /\.css$/,
                exclude: /node_modules/,
                use: [
                    'file-loader',
                    'css-loader'
                ]
            }, /*webpack reads end-to-start*/
            {
                test: /\.s[ac]ss$/i,
                exclude: /node_modules/,
                use: [
                    "file-loader",
                    "css-loader",
                    "sass-loader",
                ],
            },
            {
                test: /\.m?jsx?$/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                    options: {
                        presets: ['@babel/preset-env', '@babel/preset-react']
                    }
                }
            },
            {
                test: /\.svg$/,
                exclude: /node_modules/,
                use: ['@svgr/webpack'],
            },
        ]
    },
    plugins: [new LoadablePlugin({
        writeToDisk: true,
        filename: "loadable-stats.json",
    })
    ],

    resolve: {
        extensions: ["", ".webpack.js", ".web.js", ".jsx", ".js"]
    },
};

module.exports = webpackConfig;