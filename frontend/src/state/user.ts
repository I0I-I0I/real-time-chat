import { IUser } from "@/types"
import { create } from "zustand"

interface UserStore {
    auth: boolean;
    data: IUser | null,
    setUser: (data: IUser | null) => void,
    setAuth: (auth: boolean) => void
}

export const useUserStore = create<UserStore>()((set) => ({
    auth: false,
    data: null,
    setUser: (data: IUser | null) => set({ data }),
    setAuth: (auth: boolean) => set({ auth })
}))
