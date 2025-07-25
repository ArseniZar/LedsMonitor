<template>
  <div className=" w-10/12  p-4 pt-6  mx-auto max-w-md min-h-screen">
    <Alert />
    <BottomBar
      v-if="![StepMain.Startup, StepMain.AddDevices, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:visible="stepStack.length > 0 ? stepStack[stepStack.length - 1] : StepMain.Startup" v-on:goBack=""
      v-on:goMain="() => hadleCheckDataAndStepChange(NavigationCommand.OpenColorPanel)"
      v-on:goSettings="hadleCheckDataAndStepChange(NavigationCommand.OpenSettings)" />

    <MainStartup v-if="[StepMain.Startup].includes(stepStack[stepStack.length - 1])"
      v-on:tokenAndBotId="(data) => { handleAddTokenAndBotId(data); hadleCheckDataAndStepChange(NavigationCommand.OpenColorPanel) }" />

    <MainAddDevice v-if="[StepMain.AddDevices].includes(stepStack[stepStack.length - 1])"
      v-on:goBack="hadleCheckDataAndStepChange(NavigationCommand.GoBack)"
      v-on:newDevices="(devices) => { handleAddDevices(devices); hadleCheckDataAndStepChange(NavigationCommand.OpenColorPanel); }" />

    <MainColorPanel v-if="[StepMain.Main, StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:device="getCurrentDevice!" v-on:updateDevice="handleUpdateDevice"
      v-on:selectDevice="hadleCheckDataAndStepChange(NavigationCommand.OpenSelectDevice)" />

    <MainSelectDevice v-if="[StepMain.Select].includes(stepStack[stepStack.length - 1])"
      v-bind:devices="getCurrentChatDevices!"
      v-on:deviceSelected="(device) => { handleSetCurrentDevice(device); hadleCheckDataAndStepChange(NavigationCommand.OpenColorPanel); }"
      v-on:goBack="hadleCheckDataAndStepChange(NavigationCommand.GoBack)" />

    <MainSettings v-if="stepStack[stepStack.length - 1] === StepMain.Settings" v-bind:currentChat="getCurrentChat!"
      v-bind:devices="getCurrentChatDevices!" v-bind:itemsChat="getAllChats!"
      v-on:goBack="hadleCheckDataAndStepChange(NavigationCommand.GoBack)"
      v-on:chatToAdd="hadleCheckDataAndStepChange(NavigationCommand.OpenAddDevice)"
      v-on:chatToSelect="handleSetCurrentChat" v-on:deviceToRemove="handleRemoveDevice"
      v-on:chatToRemove="handleRemoveChat" v-on:tokenToRemove="handleRemoveToken" />



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
import Alert from "./components/basic/Alert.vue";


import { CONFIG } from "./basic/config";
import { NavigationCommand } from "./navigationCommand";
import { LocalDB, LocalConfig } from "./basic/utils/storage";
import { StepMain } from "./basic/types/steps";
import { Device } from "./basic/classes/Device";
import { Chat } from "./basic/classes/Chat";
import { DeviceController } from "./basic/classes/DeviceController"
import { scheduleSendDevice } from './sendDevice'



import type { ConfigType } from "./basic/config";
import type { NavigationCommandValue } from "./navigationCommand";




export default {
  name: "App",
  components: {
    MainSelectDevice,
    MainColorPanel,
    MainAddDevice,
    MainStartup,
    MainSettings,
    BottomBar,
    Alert,
  },

  data(): {
    deviceController: DeviceController | null,
    devicesDB: LocalDB<Device> | null;
    configDB: LocalConfig<ConfigType> | null;
    tg: WebApp | null;

    stepStack: StepMain[];
    StepMain: typeof StepMain;
    NavigationCommand: typeof NavigationCommand;
    CONFIG: ConfigType;
  } {

    return {
      deviceController: null,
      devicesDB: null,
      configDB: null,
      tg: null,
      CONFIG: CONFIG,
      stepStack: [],
      StepMain: StepMain,
      NavigationCommand: NavigationCommand,

    };
  },

  computed: {
    getCurrentDevice(): Device | null {
      if (this.deviceController) {
        const result = this.deviceController!.getCurrentDevice();
        return result;
      }
      return null;
    },

    getCurrentChat(): Chat | null {
      if (this.deviceController) {
        const result = this.deviceController.getCurrentChat();
        return result;
      }
      return null;

    },

    getCurrentChatDevices(): Device[] | null {
      if (this.deviceController) {
        const result = this.deviceController!.getCurrentChatDevices();
        return result;
      }
      return null;
    },

    getAllChats(): Chat[] | null {
      if (this.deviceController) {
        const result = this.deviceController!.getAllChats();
        return result;
      }
      return null;
    },
  },

  watch: {
    deviceController: {
      handler() {
        this.hadleCheckDataAndStepChange(NavigationCommand.NoChange);
      },
      deep: true
    },

    CONFIG: {
      handler() {
        this.hadleCheckDataAndStepChange(NavigationCommand.NoChange);
      },
      deep: true
    }
  },



  created() {
    this.initializeApp();
  },

  beforeDestroy() {
    this.destroyApp();
  },

  methods: {
    hadleCheckDataAndStepChange(id: NavigationCommandValue) {
      if (!CONFIG.token || CONFIG.token === '' || !CONFIG.botId || CONFIG.botId === '') {
        this.stepStackChange(NavigationCommand.OpenStartup);
        return;
      }

      if (
        !this.deviceController!.hasCurrentChat() ||
        !this.deviceController!.hasCurrentDevice() ||
        !this.deviceController!.hasAnyChats() ||
        !this.deviceController!.hasAnyDevices()
      ) {
        this.stepStackChange(NavigationCommand.OpenAddDevice);
        return;
      }

      if (id !== NavigationCommand.NoChange) {
        this.stepStackChange(id);
        return;
      }
    },


    stepStackChange(id: NavigationCommandValue) {
      switch (id) {
        case NavigationCommand.OpenStartup:
          this.setStep(StepMain.Startup);
          return;

        case NavigationCommand.OpenColorPanel:
          this.setStep(StepMain.Main);
          return;
        case NavigationCommand.OpenAddDevice:
          this.setStep(StepMain.AddDevices);
          return;
        case NavigationCommand.OpenSettings:
          this.setStep(StepMain.Settings);
          return;
        case NavigationCommand.OpenSelectDevice:
          this.setStep(StepMain.Select);
          return;

        case NavigationCommand.GoBack:
          if (this.stepStack.length - 1 > 0) {
            this.stepStack.pop();
          }
          return;
      }
    },



    handleRemoveDevice(removeToDelete: Device) {
      this.deviceController!.removeDevice(removeToDelete);
      this.commitDB();
    },

    handleRemoveChat(removeToChat: Chat) {
      this.deviceController!.removeChat(removeToChat);
      this.commitDB();
    },

    handleSetCurrentChat(chat: Chat) {
      this.deviceController!.setCurrentChat(chat);
    },


    handleSetCurrentDevice(device: Device) {
      this.deviceController!.setCurrentDevice(device);
    },


    handleUpdateDevice(device: Device) {
      this.deviceController!.updateDevice(device);
      scheduleSendDevice(this.deviceController!.getCurrentDevice()!);

    },


    handleAddDevices(devices: Device[]) {
      for (const device of devices) {
        this.deviceController!.addDevice(device);
      }
      this.commitDB();
    },


    handleAddTokenAndBotId(data: { token: string; botId: string }) {
      CONFIG.token = data.token;
      CONFIG.botId = data.botId;
      this.configDB!.set("botId", CONFIG.botId);
      this.configDB!.set("token", CONFIG.token);
    },

    handleRemoveToken() {
      CONFIG.token = '';
      CONFIG.botId = '';
      this.configDB!.set("token", CONFIG.token);
      this.configDB!.set("botId", CONFIG.botId);
    },

    commitDB() {
      const allDevices = this.deviceController!.getAllDevices();
      this.devicesDB!.replaceAll(allDevices);
    },


    setStep(stepMain: StepMain) {
      const currentStep = this.stepStack[this.stepStack.length - 1];
      if (currentStep !== stepMain) {
        this.stepStack.push(stepMain);
      }
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
      this.hadleCheckDataAndStepChange(NavigationCommand.OpenColorPanel);
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
        this.tg!.close();
      });
    },


    destroyTelegramApp() {
      window.removeEventListener("visibilitychange", () => { this.commitDB() });
      this.tg!.BackButton.offClick(() => {
        this.commitDB();
        this.tg!.close();
      });
    },


    initializeWebApp() {
      window.addEventListener("beforeunload", () => { this.commitDB() });
    },


    destroyWebApp() {
      window.removeEventListener("beforeunload", () => { this.commitDB() });
    },


    initializeDataBase() {
      this.devicesDB = new LocalDB<Device>("devices", ["id", "chat.id"], Device);
      this.configDB = new LocalConfig<ConfigType>("CONFIG");

      const savedDevices: Device[] = this.devicesDB.getAll();
      const savedBotId: string = String(this.configDB.getAll().botId);
      const savedToken: string = String(this.configDB.getAll().token);

      if (savedBotId !== "undefined" && savedToken !== "undefined" && savedBotId.trim() && savedToken.trim()) {
        CONFIG.botId = savedBotId;
        CONFIG.token = savedToken;
      }

      if (savedDevices !== undefined && savedDevices.length > 0) {
        this.deviceController = new DeviceController(savedDevices);
      } else {
        this.deviceController = new DeviceController();
      }
    },

  },
};
</script>