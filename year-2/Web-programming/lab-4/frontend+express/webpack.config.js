const path = require('path');
const webpack = require('webpack');
const ringConfig = require('@jetbrains/ring-ui/webpack.config').config;
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyWebpackPlugin = require("copy-webpack-plugin");

process.env.NODE_ENV = 'production';

const webpackConfig = {
    // режим webpack оптимизации
    mode: 'development' === process.env.NODE_ENV ? 'development' : 'production',

    entry: path.resolve('src/index.js'),

    // выходные файлы и чанки
    output: {
        path: path.resolve('dist/assets'),
        filename: '[name].js',
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
                    process.env.NODE_ENV !== "production"
                        ? "style-loader"
                        : MiniCssExtractPlugin.loader,
                    'css-loader'
                ]
            }, /*webpack reads end-to-start*/
            {
                test: /\.s[ac]ss$/i,
                use: [
                    // fallback to style-loader in development
                    process.env.NODE_ENV !== "production"
                        ? "style-loader"
                        : MiniCssExtractPlugin.loader,
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
                use: ['@svgr/webpack'],
            },
        ]
    },
    plugins: [
        new HtmlWebpackPlugin({
            title: 'Web-lab-4-front',
            template: path.resolve('public/template.html'),
            minify: false,
        }),
        new MiniCssExtractPlugin({
            filename: 'styles.css'
        }),
        new CopyWebpackPlugin({
            patterns: [
                {
                    from: path.resolve('public/assets'),
                    to: path.resolve('dist/assets'),
                }
            ]
        }),
    ],
    resolve: {
        extensions: ["", ".webpack.js", ".web.js", ".jsx", ".js"]
    },
    // webpack оптимизации
    optimization: {
        splitChunks: {
            cacheGroups: {
                default: false,
                vendors: false,

                vendor: {
                    chunks: 'all', // both : consider sync + async chunks for evaluation 
                    name: 'vendor', // имя чанк-файла
                    test: /node_modules/, // test regular expression
                }
            }
        }
    },
    // настройки сервера разработки
    devServer: {
        port: 8088,
        historyApiFallback: true,
    },

    // генерировать source map
    devtool: 'source-map'
}

module.exports = webpackConfig;