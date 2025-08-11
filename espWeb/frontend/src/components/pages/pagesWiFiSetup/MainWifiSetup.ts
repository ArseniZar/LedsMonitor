import { useState } from "preact/hooks";
import { html } from "../../../html";
import { MainWifiSetupAuto } from "./pagesWifiSetupAuto/MainWifiSetupAuto";
import { MainWifiSetupManual } from "./pagesWifiSetupManual/MainWifiSetupManual";
import { SelectedOption, StepWifiSetup } from "../../../basic/types/Step";
import type { Callback } from "../../../basic/types/Callback";
import type { Network } from "../../../basic/classes/Network";
import { MainWifiSetupConnect } from "./pagesWifiSetupConnect/MainWifiSetupConnect";

export function MainWifiSetup() {
  // prettier-ignore
  const [network, setNetwork] = useState<Network | null>(null);
  // prettier-ignore
  const [selected, setSelected] = useState<SelectedOption>(SelectedOption.Option1);
  // prettier-ignore
  const [stepStack, setStep] = useState<StepWifiSetup[]>([StepWifiSetup.Start]);

  function onChange(e: Event) {
    const target = e.target as HTMLInputElement;
    setSelected(target.value as SelectedOption);
  }

  function popStep() {
    setStep((prevSteps) => prevSteps.slice(0, -1));
  }

  function pushStep(newStep: StepWifiSetup) {
    setStep((prevSteps) => [...prevSteps, newStep]);
  }

  function handleGoBack() {
  if (stepStack.length - 1 > 0) {
    popStep();
    }
  }

  function hadleConnect(network: Network) {
    setNetwork(network);
    pushStep(StepWifiSetup.Connect);
  }

  function hadleConnected() {
    pushStep(StepWifiSetup.End);
    window.close();
  }

  // prettier-ignore
  function renderSelectSwitch(props: {value: SelectedOption, selected: string, onChange: Callback}) {
    return html` 
      <label class="w-full">
        <input
          onChange=${onChange}
          checked=${props.selected === props.value}
          type="radio"
          name="select"
          class="hidden peer"
          value=${props.value}
        />
        <div class="${props.selected === props.value ? "bg-white": "bg-black"}  w-full h-3 rounded-3xl  transition duration-300 transform 
            peer-checked:scale-105  hover:scale-105  peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]">  
          
        </div>
      </label>
    `;
  }

  // prettier-ignore
  return html`
   
        <div class="h-full w-full flex items-center justify-center">
          <div class="py-4 w-full aspect-square flex flex-col  md:px-5 items-center bg-black/30  rounded-3xl shadow-3xl">
            ${[StepWifiSetup.Start].includes(stepStack[stepStack.length - 1])  && 
              html`
                <div class="px-10 w-full flex flex-row gap-4 justify-center">
                  <${renderSelectSwitch} value=${SelectedOption.Option1} selected=${selected} onChange=${onChange}/>
                  <${renderSelectSwitch} value=${SelectedOption.Option2} selected=${selected} onChange=${onChange}/>
                </div>
                ${selected == SelectedOption.Option1 && html`<${MainWifiSetupAuto} connectEmit=${hadleConnect} goBack=${handleGoBack} />`}
                ${selected == SelectedOption.Option2 && html`<${MainWifiSetupManual} connectEmit=${hadleConnect} goBack=${handleGoBack} />`}
              `
            }
            ${[StepWifiSetup.Connect].includes(stepStack[stepStack.length - 1]) &&  html` <${MainWifiSetupConnect} network=${network} connectedEmit=${hadleConnected} goBack=${handleGoBack}  />`}
             ${[StepWifiSetup.End].includes(stepStack[stepStack.length - 1]) &&  
              html`
                <div class="flex-1 flex items-center justify-center">
                  <span class="text-center text-md font-mono text-white/90 shadow-2xl"> Please close this page </span>
                </div>
                `}
          </div>
        </div>
    
    
  `;
}
