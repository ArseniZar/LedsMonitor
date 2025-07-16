<template>
  <div className=" w-10/12  p-4 pt-6  mx-auto max-w-md min-h-screen">
    <BottomBar
      v-if="![StepMain.Startup, StepMain.AddDevices, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:visible="visible" v-on:goBack="" v-on:goMain="() => handleStepStackChange(ComponentId.BottomBarMain)"
      v-on:goSettings="handleStepStackChange(ComponentId.BottomBarSettings)" />

    <MainStartup v-if="[StepMain.Startup].includes(stepStack[stepStack.length - 1])"
      v-on:tokenAndBotId="(data) => { handleMainStartupTokenAndBotId(data); handleStepStackChange(ComponentId.MainStartup) }" />

    <MainAddDevice v-if="[StepMain.AddDevices].includes(stepStack[stepStack.length - 1])"
      v-on:goBack="handleStepStackChange(ComponentId.GoBack)" v-on:newOffset="handleOffset"
      v-on:newDevices="(devices) => { handleUpdateDevices(devices); handleStepStackChange(ComponentId.MainAddDevice); }" />

    <MainColorPanel v-if="[StepMain.Main, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:device="currentDevice" v-on:updateDevice="handleUpdateDevice"
      v-on:selectDevice="handleStepStackChange(ComponentId.MainColorPanel)" />

    <MainSelectDevice v-if="[StepMain.Select].includes(stepStack[stepStack.length - 1])" v-bind:devices="devices"
      v-on:deviceSelected="(device) => { handleCurrentDevice(device); handleStepStackChange(ComponentId.MainSelectDevice); }"
      v-on:goBack="handleStepStackChange(ComponentId.GoBack)" />

    <MainSettings v-if="
      stepStack[stepStack.length - 1] === StepMain.Settings" v-on:goBack="handleStepStackChange(ComponentId.GoBack)" />



    <!-- <Transition
    mode="out-in"
    enter-active-class="transition-opacity duration-300 ease-out"
    enter-from-class="opacity-0 translate-y-2"
    enter-to-class="opacity-100 translate-y-0"
    leave-active-class="transition-opacity duration-200 ease-in"
    leave-from-class="opacity-100 translate-y-0"
    leave-to-class="opacity-0 translate-y-2"
  >
    <component
      :is="currentStepComponent"
      :devices="devices"
      @deviceSelected="deviceSelected"
      @addDevice="handleAddDevice"
      @goBack="handleGoBack"
      @deviceCreate="handleDeviceCreate"
      :key="stepStack[stepStack.length - 1]"
    />
  </Transition> -->
  </div>
</template>

<script lang="ts">
import { markRaw } from "vue";
import MainStartup from "./components/pages/pagesStartup/MainStartup.vue";
import MainAddDevice from "./components/pages/pagesAddDevice/MainAddDevice.vue";
import MainSelectDevice from "./components/pages/pagesSelectDevice/pageSelectDevice/MainSelectDevice.vue";
import MainColorPanel from "./components/pages/pagesColorPanel/pageColorPanel/MainColorPanel.vue";
import MainSettings from "./components/pages/pagesSettings/pageSettings/MainSettings.vue";

import BottomBar from "./components/basic/header/BottomBar.vue";

import { CONFIG } from "./config";
import { ComponentId } from "./componentId";
import { LocalDB, LocalConfig } from "./storage";
import { StepMain, Device } from "./interface";


import type { ConfigType } from "./config";
import type { ComponentIdValueType } from "./componentId";

export default {
  name: "App",
  components: {
    MainSelectDevice,
    MainColorPanel,
    MainAddDevice,
    MainStartup,
    MainSettings,

    BottomBar,
  },
  data(): {
    visible: 'back' | 'main' | 'settings',
    devices: Device[];
    currentDevice: Device;
    stepStack: StepMain[];
    StepMain: typeof StepMain;
    ComponentId: typeof ComponentId;
    devicesDB: LocalDB<Device>;
    configDB: LocalConfig<ConfigType>;
    tg: WebApp;
  } {
    const devicesDB = new LocalDB<Device>("devices", ["id", "chat.id"], Device);
    const configDB = new LocalConfig<ConfigType>("CONFIG");
    return {
      visible: 'main',
      devices: [],
      currentDevice: {} as Device,
      stepStack: [],
      devicesDB: devicesDB,
      configDB: configDB,
      tg: {} as WebApp,
      StepMain: StepMain,
      ComponentId: ComponentId,
    };
  },
  watch: {
    stepStack: {
      immediate: true,
      deep: true,
      handler(newStack: any[]) {
        const lastStep = newStack[newStack.length - 1];
        switch (lastStep) {
          case StepMain.Main:
            this.visible = 'main';
            break;
          case StepMain.Settings:
            this.visible = 'settings';
            break;
          default:
            this.visible = 'main';
            break;
        }
      }
    }
  },

  mounted() {
    this.initializeApp();
    console.log(this.devices)
  },

  beforeDestroy() {
    this.destroyApp();
  },

  methods: {

    handleStepStackChange(id: ComponentIdValueType) {
      const hasToken = CONFIG.token && CONFIG.token !== "" && CONFIG.token !== "undefined";
      const hasBotId = CONFIG.botId && CONFIG.botId !== "" && CONFIG.botId !== "undefined";
      const hasDevices = this.devices.length > 0;

      if (!hasToken || !hasBotId) {
        this.setStep(StepMain.Startup);
        return;
      }

      if (!hasDevices) {
        this.setStep(StepMain.AddDevices);
        return;
      }

      switch (id) {
        case ComponentId.MainStartup:
          this.setStep(StepMain.Main);
          return;

        case ComponentId.BottomBarMain:
          this.setStep(StepMain.Main);
          return;
        case ComponentId.BottomBarSettings:
          this.setStep(StepMain.Settings);
          return;

        case ComponentId.GoBack:
          if (this.stepStack.length - 2 > 0) {
            this.stepStack.pop();
          }
          return;
        case ComponentId.MainColorPanel:
          this.setStep(StepMain.Select);
          return;


        case ComponentId.MainSelectDevice:
          this.setStep(StepMain.Main);
          return;

        case ComponentId.MainAddDevice:
          this.setStep(StepMain.Main);
          return;
      }




    },

    handleCurrentDevice(device: Device) {
      const exists = this.devices.some((d) => d.equals(device));
      const isSame = this.currentDevice?.equals?.(device);

      if (isSame) {
        return;
      }
      if (exists) {
        this.currentDevice = device;
      }
    },


    handleUpdateDevice(device: Device) {
      const index = this.devices.findIndex((d) => d.equals(device));
      if (index !== -1) {
        this.devices[index] = device;
        this.currentDevice = device;
      }
    },

    handleUpdateDevices(devices: Device[]) {
      console.log(devices)
      devices.forEach(device => {
        const index = this.devices.findIndex(d => d.equals(device));
        if (index !== -1) {
          this.devices[index] = device;
        } else {
          this.devices.push(device);
        }
      });

      if (devices.length > 0) {
        this.currentDevice = devices[devices.length - 1];
      }
      this.commitDB();
    },



    commitDB() {
      this.devicesDB.replaceAll(this.devices);
    },

    setStep(StepMain: StepMain) {
      this.stepStack.push(StepMain);
    },

    handleMainStartupTokenAndBotId(data: { token: string; chatId: string }) {
      CONFIG.token = data.token;
      CONFIG.botId = data.token;
      this.configDB.set("botId", CONFIG.token);
      this.configDB.set("token", CONFIG.botId);
    },

    handleOffset(offset: string) {
      CONFIG.offset = offset;
      this.configDB.set("offset", CONFIG.offset);
    },

    initializeApp() {
      if (
        typeof window.Telegram?.WebApp?.initData === "string" &&
        window.Telegram.WebApp.initData.length > 0
      ) {
        this.initializeTelegramApp();
      } else {
        console.log("web")
        this.initializeWebApp();
      }
      this.initializeDataBase();
      this.handleStepStackChange(ComponentId.BottomBarMain);
    },

    destroyApp() {
      if (this.tg) {
        this.destroyTelegramApp();
      } else {
        this.destroyWebApp();
      }
    },

    initializeTelegramApp() {
      this.tg = markRaw(Telegram.WebApp);
      this.tg.ready();
      this.tg.BackButton.show();
      window.addEventListener("visibilitychange", () => { this.commitDB() });

      this.tg.BackButton.onClick(() => {
        this.commitDB();
        this.tg.close();
      });
    },

    destroyTelegramApp() {
      window.removeEventListener("visibilitychange", () => { this.commitDB() });
      this.tg.BackButton.offClick(() => {
        this.commitDB();
        this.tg.close();
      });
    },

    initializeWebApp() {
      window.addEventListener("beforeunload", () => { this.commitDB() });
    },

    destroyWebApp() {
      window.removeEventListener("beforeunload", () => { this.commitDB() });
    },

    initializeDataBase() {
      const savedDevices: Device[] = this.devicesDB.getAll();
      const savedBotId: string = String(this.configDB.getAll().botId).valueOf();
      const savedToken: string = String(this.configDB.getAll().token).valueOf();
      const savedOffset: string = String(
        this.configDB.getAll().offset
      ).valueOf();

      if (savedBotId?.trim() && savedToken?.trim()) {
        CONFIG.botId = savedBotId;
        CONFIG.token = savedToken;
        CONFIG.offset = savedOffset;
      }

      if (savedDevices && savedDevices.length > 0) {
        this.devices = savedDevices;
        this.currentDevice = this.devices[0];
      }
    },


  },
};
</script>
