import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  connect(relayUrl: string): void;
  subscribe(filter: string): void;
  sendEvent(event: string): void;
}

export default TurboModuleRegistry.getEnforcing<Spec>('NostrClient'); 