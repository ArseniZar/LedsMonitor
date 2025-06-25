<template>
  <div className=" w-10/12  p-4 pt-10  mx-auto max-w-md min-h-screen">

    <div className="flex  items-center">
      <button @click="selectDevice"
        className="text-center h-8 font w-full shadow-md text-white/90 mx-auto p-1 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block">
        {{ currentDevice.name }}</button>
    </div>
    <Welcome v-if="stepStack[stepStack.length - 1] === StepMain.Welcome" v-on:addDevice="handleAddDevice" />
    <ColorPanel
      v-if="stepStack[stepStack.length - 1] === StepMain.Main || stepStack[stepStack.length - 1] === StepMain.Select"
      v-bind:device="currentDevice" v-on:updateDevice="updateDevice" />
    <DeviceMain v-if="stepStack[stepStack.length - 1] === StepMain.Select" v-bind:devices="devices"
      v-on:deviceSelected="deviceSelected" v-on:addDevice="handleAddDevice" v-on:goBack="handleGoBack" />
    <DeviceAddMain v-if="stepStack[stepStack.length - 1] === StepMain.Add" v-on:deviceCreate="handleDeviceCreate"
      v-on:goBack="handleGoBack" />


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


export default {
  name: 'App',
  components: {
    DeviceMain,
    DeviceAddMain,
    ColorPanel,
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
    };
  },


  mounted() {
    this.initializeApp();
    window.addEventListener('beforeunload', this.handleBeforeUnload);
  },

  beforeMount() {
    window.removeEventListener('beforeunload', this.handleBeforeUnload);
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
    handleGoBack() {
      console.log('Go back clicked');
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
        console.log(this.currentDevice)
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


  }
}


</script>
