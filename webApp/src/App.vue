<template>
  <div className=" w-10/12  p-4 pt-10  mx-auto max-w-md min-h-screen">
    <Welcome v-if="stepStack[stepStack.length - 1] === StepMain.Welcome" v-on:addDevice="handleAddDevice" />
    <ButtonSelect
      v-if="stepStack[stepStack.length - 1] === StepMain.Main || stepStack[stepStack.length - 1] === StepMain.Select"
      v-bind:device="currentDevice" v-on:selectDevice="selectDevice" />
    <ColorPanel
      v-if="stepStack[stepStack.length - 1] === StepMain.Main || stepStack[stepStack.length - 1] === StepMain.Select"
      v-bind:device="currentDevice" v-on:updateDevice="updateDevice" />
    <DeviceMain v-if="stepStack[stepStack.length - 1] === StepMain.Select" v-bind:devices="devices"
      v-on:deviceSelected="deviceSelected" v-on:addDevice="handleAddDevice" v-on:deleteDevice="handleDeleteDevice"
      v-on:goBack="handleGoBack" />
    <DeviceAddMain v-if="stepStack[stepStack.length - 1] === StepMain.Add" v-bind:devices="devices"
      v-on:deviceCreate="handleDeviceCreate" v-on:goBack="handleGoBack" />


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

import type { Device, BoxColor } from './interface';
import { StepMain } from './interface';
import { LocalDB } from './storage';
import DeviceMain from './components/device/DeviceMain.vue';
import DeviceAddMain from './components/device/deviceAdd/DeviceAddMain.vue';
import ColorPanel from './components/main/ColorPanel.vue';
import Welcome from './components/Welcome.vue';
import ButtonSelect from './components/main/ButtonSelect.vue';


export default {
  name: 'App',
  components: {
    DeviceMain,
    DeviceAddMain,
    ColorPanel,
    ButtonSelect,
    Welcome,
  },
  data(): {
    statusLed: Boolean,
    devices: Device[];
    boxColors: BoxColor[];
    currentDevice: Device;
    stepStack: StepMain[];
    StepMain: typeof StepMain;
    devicesDB: LocalDB<Device>;
    tg: WebApp;
  } {
    const devicesDB = new LocalDB<Device>('devices', 'token');
    return {
      statusLed: false,
      StepMain: StepMain,
      devices: [],
      boxColors: [],
      currentDevice: {} as Device,
      stepStack: [],
      devicesDB: devicesDB,
      tg: {} as WebApp,
    };
  },


  mounted() {

    this.initializeApp();
    this.initializeTelegramApp();
    // window.addEventListener('beforeunload', this.handleBeforeUnload);
  },

  beforeDestroy() {
    if (this.tg) {
      this.tg.BackButton.offClick(this.handleBeforeUnload);
    }
  },

  computed: {
    currentStepComponent() {
      const currentStep = this.stepStack[this.stepStack.length - 1];
      if (currentStep === StepMain.Select) return 'DeviceMain';
      if (currentStep === StepMain.Add) return 'DeviceAddMain';
      return null;
    }
  },
  methods: {
    deviceSelected(device: Device) {
      this.currentDevice = device;
      this.stepStack.push(StepMain.Main);
    },
    handleAddDevice() {
      this.stepStack.push(StepMain.Add);
    },

    handleDeleteDevice(device: Device) {
      this.devices = this.devices.filter(d => d.token !== device.token);

      if (this.devices.length === 0) {
        this.stepStack = [StepMain.Welcome];
        this.currentDevice = {} as Device;
      } else if (this.currentDevice.token === device.token) {
        this.currentDevice = this.devices[0];
      }
    },

    handleGoBack() {
      this.stepStack.pop()
    },

    handleDeviceCreate(device: Device) {
      this.stepStack.pop();
      this.currentDevice = device;
      this.devices.push(device);
      this.stepStack = [StepMain.Main];
    },
    selectDevice() {
      this.stepStack.push(StepMain.Select);
    },


    updateDevice(device: Device) {
      const index = this.devices.findIndex(d => d.token === device.token);
      if (index !== -1) {
        this.devices[index] = device;
        this.currentDevice = device;
      }

    },

    handleBeforeUnload() {
      this.devicesDB.replaceAll(this.devices);
    },

    initializeApp() {
      const savedDevices = this.devicesDB.getAll();
      if (savedDevices && savedDevices.length > 0) {
        this.devices = savedDevices;
        this.currentDevice = this.devices[0];
        this.stepStack = [StepMain.Main];
      } else {
        this.stepStack = [StepMain.Welcome];
      }
    },


    initializeTelegramApp() {
      this.tg = window.Telegram?.WebApp;
      if (this.tg) {
        this.tg.ready();
        
        // Показываем кнопку "Назад"
        this.tg.BackButton.show();
        
        // Вешаем НАШ обработчик на ее нажатие
        this.tg.BackButton.onClick(this.handleBeforeUnload);

      } else {
        setTimeout(this.initializeApp, 100);
      }
    },


  }
}


</script>
