import styles from "./Dropdown.module.css"
import cls from "@/utils/cls"
import React from "react"

interface DropdownProps {
    dropdownState: boolean
    children: React.ReactNode
    className?: string
}

interface DropdownItemProps extends React.HTMLAttributes<HTMLLIElement> {
    children: React.ReactNode,
    className?: string
}

export const Dropdown = ({
    dropdownState,
    className = "",
    children
}: DropdownProps): JSX.Element => (
    <ul className={cls(className, styles.dropdown, dropdownState ? "" : "hidden" )}>
        {children}
    </ul>
)

export const DropdownItem = ({
    children,
    className = "",
    ...props
}: DropdownItemProps): JSX.Element => (
    <li className={cls(styles.dropdown_item, className)} {...props}>
        {children}
    </li>
)
