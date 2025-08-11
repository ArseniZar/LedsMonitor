import type { Network } from "../../../../../basic/classes/Network";
import { ped } from "../../../../../basic/img/img";
import type { Callback } from "../../../../../basic/types/Callback";
import { html } from "../../../../../html";

export function Check(props:{network: Network, connectEmit: Callback, goBack: Callback}) {
  function handleConnect() {
    props.connectEmit();
  }
  return html`
    <div class="w-full flex-1 flex flex-col  gap-3.5 px-6 py-4 mt-6">
      <div class="">
        <button
          onClick=${props.goBack}
          class="p-1 rounded-full hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.5)] hover:scale-110 duration-300"
        >
          <img src=${ped} alt="Check" class="h-6" />
        </button>
      </div>

      <div class="flex w-full flex-col gap-6 mx-auto px-4">
        <div class="flex w-full gap-2 pb-2 border-b-1 border-green-600">
          <span class="text-left font-bold">Name:</span>
          <span class="flex-1 text-center truncate">${props.network.ssid}</span>
        </div>
        <div class="flex w-full gap-2 pb-2 border-b-1 border-green-600">
          <span class="text-left font-bold">Password:</span>
          <span class="flex-1 text-center truncate">${props.network.password}</span>
        </div>
      </div>

      <button
        onClick=${handleConnect}
        class="mx-auto w-full  mt-5 shadow-md  p-2 bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 transition block active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]"
      >
        <span class="text-center text-md font-mono text-white/90 ">
          Connect
        </span>
      </button>
    </div>
  `;
}
