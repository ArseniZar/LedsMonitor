import "./main.css";
import { render } from "preact";
import { App } from "./App";

const appElement = document.getElementById("app");

if (appElement) {
  render(<App />, appElement);
}