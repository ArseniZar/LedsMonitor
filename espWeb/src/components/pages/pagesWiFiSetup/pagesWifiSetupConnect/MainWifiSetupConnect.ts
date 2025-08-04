import { useState } from "preact/hooks";
import { html } from "../../../../html";
import { Connect } from "./pageWifiSetupConnect/Connect.ts";
import { StepWifiSetupConnect } from "../../../../basic/types/Step.ts";
import type { Network } from "../../../../basic/classes/Network.ts";
import type { Callback } from "../../../../basic/types/Callback.ts";

export function MainWifiSetupConnect(props: { network: Network, connectedEmit: Callback, goBack: Callback }) {
  // prettier-ignore
  const [stepStack, setStep] = useState<StepWifiSetupConnect[]>([StepWifiSetupConnect.Connect]);

  function popStep() {
    setStep((prevSteps) => prevSteps.slice(0, -1));
  }

  function pushStep(newStep: StepWifiSetupConnect) {
    setStep((prevSteps) => [...prevSteps, newStep]);
  }
  function hadleConnected(){
    pushStep(StepWifiSetupConnect.End);
    props.connectedEmit();
  }

  function handleGoBack() {
    if (stepStack.length - 1 > 0) {
      popStep();
      return;
    }
    props.goBack();
  }
  // prettier-ignore
  return html`
    ${[StepWifiSetupConnect.Connect].includes(stepStack[stepStack.length - 1]) && html`<${Connect} network=${props.network} connectedEmit=${hadleConnected} goBack=${handleGoBack}/>`}
  `;
}
