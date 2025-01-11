
{ pkgs, lib, config, inputs, ... }:
{
  env.GREET = "devenv";
  env.YARN_CHECKSUM_BEHAVIOR = "reset";
  packages = [ pkgs.git ];
  android = {
    platforms.version = [ "35" "34" ];
    enable = true;
    reactNative.enable = true;
    android-studio.enable = false;
    platformTools.version = "35.0.2";
    buildTools.version = [ "35.0.0" "34.0.0" ];
  };
  languages.javascript.enable = true;
  languages.javascript.bun.enable = true;
  # languages.javascript.corepack.enable = true;
  # languages.typescript.enable = true;

  # scripts = {
  #   android-init.exec = ''
  #     # Install dependencies
  #     yarn install
  #
  #     # Generate proto files
  #     yarn gen-proto
  #
  #     node node_modules/react-native-turbo-lnd/src/fetch-lnd.js
  #     echo "sdk.dir=$ANDROID_HOME" > android/local.properties
  #
  #     # Download the Lndmobile.aar file and place it in the directory (overwriting if it exists)
  #     curl -L "https://github.com/hsjoberg/blixt-wallet/releases/download/v0.7.0/Lndmobile.aar" -o android/app/lndmobile/Lndmobile.aar
  #
  #     echo "Lndmobile.aar downloaded and placed in android/app/lndmobile/"
  #   '';
  #
  #   android-unsigned-apk.exec = ''
  #     yarn android:mainnet-unsigned
  #   '';
  # };
}
