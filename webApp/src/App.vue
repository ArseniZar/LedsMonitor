<template>
  <div className=" w-10/12  p-4 pt-6  mx-auto max-w-md min-h-screen">
    <BottomBar
      v-if="![StepMain.Startup, StepMain.AddDevices, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:visible="visible" v-on:goBack="" v-on:goMain="() => handleStepStackChange(ComponentId.BottomBarMain)"
      v-on:goSettings="handleStepStackChange(ComponentId.BottomBarSettings)" />

    <MainStartup v-if="[StepMain.Startup].includes(stepStack[stepStack.length - 1])"
      v-on:tokenAndBotId="(data) => { handleMainStartupTokenAndBotId(data); handleStepStackChange(ComponentId.MainStartup) }" />

    <MainAddDevice v-if="[StepMain.AddDevices].includes(stepStack[stepStack.length - 1])"
      v-on:goBack="handleStepStackChange(ComponentId.GoBack)"
      v-on:newDevices="(devices) => { handleUpdateDevices(devices); handleStepStackChange(ComponentId.MainAddDevice); }" />

    <MainColorPanel v-if="[StepMain.Main, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:device="currentDevice" v-on:updateDevice="handleUpdateDevice"
      v-on:selectDevice="handleStepStackChange(ComponentId.MainColorPanel)" />

    <MainSelectDevice v-if="[StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:devices="devicesByChat[currentChat.id]"
      v-on:deviceSelected="(device) => { handleSetCurrentDevice(device); handleStepStackChange(ComponentId.MainSelectDevice); }"
      v-on:goBack="handleStepStackChange(ComponentId.GoBack)" />

    <MainSettings v-if="
      stepStack[stepStack.length - 1] === StepMain.Settings" v-bind:currentChat="currentChat"
      v-bind:devices="devicesByChat[currentChat.id]" v-bind:itemsChat="getChatsFromDevicesByChat()"
      v-on:goBack="handleStepStackChange(ComponentId.GoBack)" v-on:deleteDevice="removeDevice"
      v-on:addChat="handleStepStackChange(ComponentId.MainSettings)" v-on:selectChat="hadleSetCurrentChat" />



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
import MainColorPanel from "./components/pages/pagesColorPanel/MainColorPanel.vue";
import MainSettings from "./components/pages/pagesSettings/MainSettings.vue";
import BottomBar from "./components/basic/header/BottomBar.vue";

import { CONFIG } from "./config";
import { ComponentId } from "./componentId";
import { LocalDB, LocalConfig } from "./storage";
import { StepMain, Device } from "./interface";
import { scheduleSendDevice } from './sendDevice'



import type { ConfigType } from "./config";
import type { ComponentIdValueType } from "./componentId";
import type { Chat } from "./telegram/types";

import Device1 from "./components/pages/pagesSettings/pageDevice/DeviceList.vue";


export default {
  name: "App",
  components: {
    MainSelectDevice,
    MainColorPanel,
    MainAddDevice,
    MainStartup,
    MainSettings,
    Device1,
    BottomBar,
  },
  data(): {
    visible: 'back' | 'main' | 'settings' | 'off',
    devicesByChat: Record<string, Device[]>;
    currentDevice: Device;
    currentChat: Chat;
    stepStack: StepMain[];
    StepMain: typeof StepMain;
    ComponentId: typeof ComponentId;
    devicesDB: LocalDB<Device>;
    configDB: LocalConfig<ConfigType>;
    Tg: WebApp;
  } {
    const devicesDB = new LocalDB<Device>("devices", ["id", "chat.id"], Device);
    const configDB = new LocalConfig<ConfigType>("CONFIG");
    return {
      visible: 'main',
      devicesByChat: {},
      currentDevice: {} as Device,
      currentChat: {} as Chat,
      stepStack: [],
      devicesDB: devicesDB,
      configDB: configDB,
      Tg: {} as WebApp,
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
            this.visible = 'off';
            break;
        }
      }
    }
  },

  mounted() {
    this.initializeApp();
  },

  beforeDestroy() {
    this.destroyApp();
  },

  methods: {



    hasAnyDevices(): boolean {
      return Object.values(this.devicesByChat).some(deviceList => deviceList.length > 0);
    },

    handleStepStackChange(id: ComponentIdValueType) {
      const hasToken = CONFIG.token && CONFIG.token !== "" && CONFIG.token !== "undefined";
      const hasBotId = CONFIG.botId && CONFIG.botId !== "" && CONFIG.botId !== "undefined";
      const hasDevices = this.hasAnyDevices();

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
        case ComponentId.MainSettings:
          this.setStep(StepMain.AddDevices);
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


    getChatsFromDevicesByChat(): Chat[] {
      const chats: Chat[] = [];

      for (const chatId in this.devicesByChat) {
        const devices = this.devicesByChat[chatId];
        if (devices.length > 0) {
          chats.push(devices[0].chat);
        }
      }
      return chats;
    },




    removeDevice(deviceToRemove: Device) {
      const currentChatId = this.currentChat.id;
      const devicesInChat = this.devicesByChat[currentChatId];

      const deviceIndex = devicesInChat.findIndex(d => d.equals(deviceToRemove));

      if (deviceIndex === -1) return;

      devicesInChat.splice(deviceIndex, 1);

      if (devicesInChat.length === 0) {
        delete this.devicesByChat[currentChatId];

        const allChats = Object.values(this.devicesByChat);

        if (allChats.length > 0) {
          const lastChatDevices = allChats[allChats.length - 1];
          const newCurrentChat = lastChatDevices[0].chat;
          this.hadleSetCurrentChat(newCurrentChat);
        } else {
          this.currentChat = {} as Chat;
          this.currentDevice = {} as Device;
        }
      } else {
        if (this.currentDevice.equals(deviceToRemove)) {
          this.currentDevice = devicesInChat[0];
        }
      }

      this.commitDB();
    },


    hadleSetCurrentChat(chat: Chat) {
      if (!chat || (this.currentChat && this.currentChat.id === chat.id)) return;
      this.currentChat = chat;
      const chatDevices = this.devicesByChat[chat.id];
      if (chatDevices && chatDevices.length > 0) {
        this.currentDevice = chatDevices[0];
      }
    },

    handleSetCurrentDevice(device: Device) {
      const isSame = this.currentDevice.equals(device);
      if (isSame) return;

      this.currentDevice = device;
    },



    handleUpdateDevice(device: Device) {
      const chatId = this.currentChat.id;
      const chatDevices = this.devicesByChat[chatId];


      const index = chatDevices.findIndex(d => d.equals(device));
      if (index !== -1) {
        chatDevices[index] = device;
        this.currentDevice = device;
      }

      scheduleSendDevice(device);

    },


    handleUpdateDevices(devices: Device[]) {

      const chat = devices[0].chat;

      if (!this.devicesByChat[chat.id]) {
        this.devicesByChat[chat.id] = [];
      }

      const chatDevices = this.devicesByChat[chat.id];

      devices.forEach(device => {
        const index = chatDevices.findIndex(d => d.equals(device));
        if (index !== -1) {
          chatDevices[index] = device;
        } else {
          chatDevices.push(device);
        }
      });

      this.hadleSetCurrentChat(chat);
      this.commitDB();
    },



    commitDB() {
      const allDevices = Object.values(this.devicesByChat).flat();
      this.devicesDB.replaceAll(allDevices);
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
      if (this.Tg) {
        this.destroyTelegramApp();
      } else {
        this.destroyWebApp();
      }
    },

    initializeTelegramApp() {
      this.Tg = markRaw(Telegram.WebApp);
      this.Tg.ready();
      this.Tg.BackButton.show();
      window.addEventListener("visibilitychange", () => { this.commitDB() });

      this.Tg.BackButton.onClick(() => {
        this.commitDB();
        this.Tg.close();
      });
    },

    destroyTelegramApp() {
      window.removeEventListener("visibilitychange", () => { this.commitDB() });
      this.Tg.BackButton.offClick(() => {
        this.commitDB();
        this.Tg.close();
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

      if (savedBotId !== "undefined" && savedToken !== "undefined" && savedBotId?.trim() && savedToken?.trim()) {
        CONFIG.botId = savedBotId;
        CONFIG.token = savedToken;
      }

      this.loadDevicesFromDB(savedDevices);
    },


    loadDevicesFromDB(devices: Device[]) {
      this.devicesByChat = {};

      for (const device of devices) {
        const chatId = device.chat.id;
        if (!this.devicesByChat[chatId]) {
          this.devicesByChat[chatId] = [];
        }
        this.devicesByChat[chatId].push(device);
      }

      const firstChatDevices = Object.values(this.devicesByChat)[0];

      if (firstChatDevices && firstChatDevices.length > 0) {
        this.currentChat = firstChatDevices[0].chat;
        this.currentDevice = firstChatDevices[0];
      }
    }


  },
};
</script>
