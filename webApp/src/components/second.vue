<template>
     <div className="fixed p-20 inset-0 bg-black/30 backdrop-blur-md flex flex-col items-center justify-center min-h-screen">
    <h1 className="text-center text-2xl font-bold text-white/90">Введи токен бота</h1>
    <input type="text"
      className="p-2  px-5 mt-10  bg-black  shadow-md text-white/90 rounded-2xl focus:outline-none focus:ring-2 focus:ring-gray-400 w-full max-w-xl"
      placeholder="Enter text here" />
    <p className="text-white/79  text-center p-1 mb-4 block min-h-9"></p>
    <button
      className="text-center font-bold shadow-md text-white/80 mx-auto p-2 px-3 bg-black/60 rounded-4xl  hover:bg-black/100 transition block">
      Add
      new
      device</button>
  </div>
</template>