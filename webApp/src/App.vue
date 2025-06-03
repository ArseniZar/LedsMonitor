<template>

  <!-- <div className=" w-2/3 p-5 mx-auto flex flex-col items-center justify-center min-h-screen">
    
    <div className="">
      <h1 className="text-center text-2xl font-bold">This App</h1>
    </div>


    <button
      className="text-center   mt-10 font-bold shadow-md text-white mx-auto p-2 px-3 bg-black rounded-4xl  hover:bg-gray-900 transition block">
      Add
      new
      device</button>
  </div> -->


  <!-- <div className="fixed p-20 inset-0 bg-black/30 backdrop-blur-md flex flex-col  items-center justify-center min-h-screen">
    <h1 className="text-center text-2xl font-bold text-white/90">Введи токен бота</h1>
    <input type="text"
      className="p-2  px-5 mt-8  bg-black  shadow-md text-white/90 rounded-2xl focus:outline-none focus:ring-2 focus:ring-gray-400 transition w-full max-w-xl"
      placeholder="Enter text here" />
    <p className="text-white/79  text-center p-1 mb-4 block min-h-9"></p>
    <button
      className="text-center font-bold shadow-md text-white/80 mx-auto p-2 px-3 bg-black/60 rounded-4xl  hover:bg-black/100 transition block">
      Add
      new
      device</button>
  </div> -->

  <div className=" w-8/12  p-4 pt-10  mx-auto max-w-md min-h-screen">

    <div className="flex  items-center">
      <button @click="SelectDevice"
        className="text-center h-8 font w-full shadow-md text-white/90 mx-auto p-1 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block">
        {{ currentDevice.name }}</button>
    </div>



    <!-- <div className="flex justify-center mt-10 w-full ">
      <div id="picker"></div>
    </div> -->


    <ColorPanel />

    <!-- <ColorMainBox v-bind:color="color" /> -->

    <!-- <div class="flex  w-full flex-row justify-between gap-3 mt-5 mx-auto mb-10">
      <label class="block h-6 max-w-[60px] w-full">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="block h-6 max-w-[60px] w-full">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="block h-6 max-w-[60px] w-full">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="block h-6 max-w-[60px] w-full">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="block h-6 max-w-[60px] w-full">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>
    </div>
  </div> -->



    <!-- <button
      className="text-center font w-full shadow-md text-white/90 mx-auto p-2  bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block">
      edit</button> -->

    <!-- <DeviceMain @device_selected="device_selected" v-bind:devices="devices">-->

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

import type { Device } from './interface'; // Adjust the path as necessary
import { StepMain } from './interface'; // Adjust the path as necessary
import DeviceMain from './components/device/DeviceMain.vue';
import DeviceAddMain from './components/device/deviceAdd/DeviceAddMain.vue';
import ColorPanel from './components/main/ColorPanel.vue';





export default {
  name: 'App',
  components: {
    DeviceMain,
    DeviceAddMain,
    ColorPanel
  },
  data() {
    return {

      devices: [
        { name: "Device 1", token: "1" },
        { name: "Device 2", token: "2" },
        { name: "Device 3", token: "3" },
      ] as Device[],
      currentDevice: {} as Device,
      stepStack: [StepMain.Main] as StepMain[],
      StepMain: StepMain,

      color: '#87CEEB' as string,
    };
  },

  mounted() {
    this.currentDevice = this.devices[0];
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
    },
    SelectDevice() {
      this.stepStack.push(StepMain.Select);
    },
  },
};


</script>

<!-- <script>
import { onMounted } from 'vue';
import iro from '@jaames/iro';
onMounted(() => {
const container = document.getElementById('main');
const style = window.getComputedStyle(container);
const paddingLeft = parseFloat(style.paddingLeft);
const paddingRight = parseFloat(style.paddingRight);
const contentWidth = container.offsetWidth - paddingLeft - paddingRight;


const colorPicker = new iro.ColorPicker("#picker", {
width: contentWidth,
color: "#ffffff",
margin: 50,
borderWidth: 0,
padding: 8,
handleRadius: 8,
layout: [
{ component: iro.ui.Wheel },
{ component: iro.ui.Slider, options: { sliderType: 'value', } },
]
});
});

</script> -->
