<script setup lang="ts">

</script>

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

  <div id="main" className=" w-8/12 m-4 mx-auto max-w-md min-h-screen">

    <div className="flex  items-center">
      <select
        className="p-2  rounded-4xl w-full shadow-md text-center text-white/90 bg-black/30 focus:outline-none hover:bg-black/50  transition  duration-400">
        <option className="text-white/90">Выберите вариант</option>
        <option className="text-white/90">Первый</option>
        <option className="text-white/90">Второй</option>
        <option className="text-white/90">Третий</option>
      </select>
    </div>
    <div className="flex justify-center mt-10 flex-row gap-1">
      <!-- <div className="bg-white w-50 h-50 rounded-full"></div> -->
      <!-- <div class="w-10 h-30 rounded-full bg-green-500 shadow-[0_0_10px_3px_rgba(34,197,94,0.7)]"></div> -->
      <!-- <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_7px_2px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div> -->

    </div>
    <div className="flex justify-between w-auto">
      <!-- <div className="bg-white w-50 h-50 rounded-full"></div> -->
      <!-- <div class="w-10 h-30 rounded-full bg-green-500 shadow-[0_0_10px_3px_rgba(34,197,94,0.7)]"></div> -->
      <div className="flex flex-col gap-1">
        <!-- <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div> -->

      </div>
      <div className="flex flex-col gap-1">
        <!-- <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
        <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div> -->

      </div>

    </div>
    <div className="flex justify-center flex-row gap-1">
      <!-- <div className="bg-white w-50 h-50 rounded-full"></div> -->
      <!-- <div class="w-10 h-30 rounded-full bg-green-500 shadow-[0_0_10px_3px_rgba(34,197,94,0.7)]"></div> -->
      <!-- <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-red-500 shadow-[0_0_8px_3px_rgba(239,68,68,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-yellow-400 shadow-[0_0_8px_3px_rgba(250,204,21,0.7)]"></div>
      <div class="w-6 h-6 rounded-full bg-green-500 shadow-[0_0_8px_3px_rgba(34,197,94,0.7)]"></div> -->

    </div>
    <div className="flex justify-center mt-10 w-full">
      <div id="picker"></div>
    </div>
    <label class="block mt-5 cursor-pointer ">
      <input type="radio" name="color" class="hidden peer" />
      <div
        class="bg-amber-300 h-6 rounded-xl transition duration-150 transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        <span class="text-center font-bold text-white/90 block"> 1.1.1.1</span>
      </div>
    </label>

    <!-- <div class="flex max-w-[40px justify-between">
      <div class="w-20 h-20 bg-blue-500"></div>
      <div class="w-20 h-20 bg-red-500"></div>
      <div class="w-20 h-20 bg-green-500"></div>
      <div class="w-20 h-20 bg-yellow-500"></div>
    </div> -->

    <!-- <div class="flex w-full justify-center "> -->
    <div class="grid grid-cols-5 mt-5 w-full ">
      <label class="flex justify-center cursor-pointer">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300  w-full max-w-[40px] h-6 rounded-xl transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="flex justify-center cursor-pointer">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full max-w-[40px] h-6 rounded-xl transition duration-150 transform
             peer-checked:scale-110  peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="flex justify-center cursor-pointer">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full max-w-[40px] h-6 rounded-xl transition duration-150 transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="flex justify-center cursor-pointer">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full max-w-[40px] h-6  rounded-xl transition duration-150 transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>

      <label class="flex justify-center cursor-pointer">
        <input type="radio" name="color" class="hidden peer" />
        <div
          class="bg-amber-300 w-full max-w-[40px] h-6 rounded-xl transition duration-150 transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] peer-checked:shadow-[0_0_10px_3px_rgba(250,204,21,0.8)]">
        </div>
      </label>
    </div>
    <!-- </div> -->


    <button
      className="text-center mt-40 font w-full shadow-md text-white/90 mx-auto p-2  bg-black/50 rounded-4xl  hover:bg-gray-900 transition block">
      Edit color</button>
  </div>




</template>

<script setup lang="ts">
import { onMounted} from 'vue';
import iro from '@jaames/iro';
// export default {

// };


onMounted(() => {
  const container = document.getElementById('main');
  var widtw;
  if (container) {
    widtw = container.offsetWidth;
    console.log('Ширина контейнера:', widtw);
  } else {
    console.warn('Контейнер с таким ID не найден');
  }


  const colorPicker = new iro.ColorPicker("#picker", {
    width: widtw,
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



</script>