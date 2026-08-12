// import { MainWifiSetup } from "./components/pages/pagesWiFiSetup/MainWifiSetup";
import { App as KonstaApp } from "konsta/react";
import Router, { Route } from "preact-router";
import { PATHS } from "@/routes";

import { HomePage } from "@/pages";

export const App = () => {
  return (
    <>
      <KonstaApp theme="ios" dark={true} safeAreas={true}>
        <Router>
          <Route path={PATHS.HOME} component={HomePage} />
        </Router>
      </KonstaApp>
    </>
  );
};
