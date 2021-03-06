const path = require('path');

module.exports = {
  entry: {
    example: './lib/js/example/example.js',
  },
  output: {
    path: path.join(__dirname, "bundledOutputs"),
    filename: '[name].js',
  },
};
