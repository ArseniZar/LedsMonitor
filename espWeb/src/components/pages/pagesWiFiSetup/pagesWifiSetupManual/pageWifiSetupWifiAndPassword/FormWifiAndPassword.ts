import { useState } from "preact/hooks";
import { Network } from "../../../../../basic/classes/Network";
import { html } from "../../../../../html";
import type { Callback } from "../../../../../basic/types/Callback";

export function FormWifiAndPassword(props: { networkEmit: Callback }) {
  const [password, setPassword] = useState<string>("");
  const [ssid, setSsid] = useState<string>("");

  function handleConfirm() {
    if (ssid.trim().length > 0) {
      const newNetwork = new Network({ ssid: ssid, password: password });
      props.networkEmit(newNetwork);
    }
  }

  function handleInputSsid(e: Event) {
    const target = e.target as HTMLInputElement;
    setSsid(target.value);
  }

  function handleInputPassword(e: Event) {
    const target = e.target as HTMLInputElement;
    setPassword(target.value);
  }

  // prettier-ignore
  return html`
    <div class="w-full flex-1 flex flex-col gap-6 px-6 py-8 items-center justify-center">
      <input
        type="text"
        onInput=${handleInputSsid}
        placeholder="Enter Wi-Fi name"
        class="w-full p-3 text-sm font-mono bg-black text-white/90 rounded-2xl shadow-lg"
      />
      <input
        type="text"
        onInput=${handleInputPassword}
        placeholder="Enter password"
        class="w-full p-3 text-sm font-mono bg-black text-white/90 rounded-2xl shadow-lg"
      />
      <button onClick=${handleConfirm} class="mx-auto w-full shadow-md p-3 bg-black/60 rounded-4xl duration-300 hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 transition block active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]">
        <span class="block text-center text-base font-semibold text-white tracking-wide">
          Confirm
        </span>
      </button>
    </div>
  `;
}
