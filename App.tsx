import React, { useEffect, useState } from 'react';
import { View, Text, StyleSheet, Button } from 'react-native';
import NostrClient from './specs/NostrClient';

export default function App() {
  const [status, setStatus] = useState('Not connected');

  const connectToRelay = () => {
    try {
      setStatus('Connecting...');
      NostrClient.connect('wss://relay.damus.io');
      setStatus('Connected, subscribing...');
      
      // Subscribe to recent events
      const filter = JSON.stringify({
        kinds: [1],
        limit: 100
      });

      console.log('filter', filter);
      
      NostrClient.subscribe(filter);
      setStatus('Subscribed to events');
    } catch (error) {
      setStatus(`Error: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Text style={styles.status}>Status: {status}</Text>
      <Button 
        title="Connect to Relay" 
        onPress={connectToRelay}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
  },
  status: {
    marginBottom: 20,
    fontSize: 16,
  },
});